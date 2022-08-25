#include "NugetStep.h"
#include <QXmlStreamWriter>
#include <QDomElement>
#include <qprocess.h>
#include <QDirIterator>
#include "DontNeedADOApp.h"



void NugetStep::setVersion(const std::string& a_vers)
{
	m_version = a_vers;
}

std::string NugetStep::version()const noexcept
{
	return m_version;
}


void NugetStep::setRelativePath(const std::string& a_value)
{
	m_relDir = a_value;
}

std::string NugetStep::relativePath()const noexcept
{
	return m_relDir;
}

void NugetStep::setFiles(const std::string& a_value)
{
	m_files = a_value;
}

std::string NugetStep::files()const noexcept
{
	return m_files;
}

bool NugetStep::execute(ExecuteArgs& a_args)const
{
	bool bRet = false;
	QProcess nugetProcess;
	QStringList lFiles = QString::fromLatin1(m_files).split(";");

	std::string version;
	if (!PiplineStep::isVariable(a_args.variables, m_version, version))
		version = m_version;

	// mise en forme path:
	QString path = QString::fromLatin1(a_args.workingDirectory).replace('\\', '/');
	QString relPath = QString::fromLatin1(m_relDir).replace('\\', '/');
	//nugetProcess.setWorkingDirectory(QString::fromLatin1(m_relDir));
	for (auto file : lFiles)
	{
		QString absFile = (path.endsWith('/') ? path : path + "/") + (relPath.endsWith('/') ? relPath : relPath + "/") + file;

		nugetProcess.start("powershell", QStringList() << "-Command" << QString("write-NugetPackage -Package %1 -PackageVersion %2")
			.arg(absFile).arg(QString::fromLatin1(version)));


		if (!nugetProcess.waitForStarted())
		{
			a_args.outputLog += PiplineStep::formatMessage("GENERATE NUGETS:", "NOT STARTED!", nugetProcess);
			return false;
		}
		bRet = nugetProcess.waitForFinished();

		a_args.outputLog += PiplineStep::formatMessage("GENERATE NUGETS:", nugetProcess);
	}

	// move files
	// directory where nugets will be transfered
	DontNeedADOApp* pApp = static_cast<DontNeedADOApp*>(qApp);
	QString newDir = pApp->nugetPath();
	if (newDir.isEmpty())
		newDir = (path.endsWith('/') ? path : path + "/") + (relPath.endsWith('/') ? relPath : relPath + "/");

	// transfert generated nupkg
	a_args.outputLog += PiplineStep::formatMessageNoError("TRANSFER FILES:", "");
	QDirIterator iter(QApplication::applicationDirPath(), QStringList() << "*.nupkg",
		QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
	while (iter.hasNext())
	{
		QString path = iter.next();
		QFileInfo dirInfo(path);
		if (dirInfo.isFile())
		{
			QFile file(path);
			file.copy(newDir + dirInfo.fileName());
			file.remove();
			a_args.outputLog += PiplineStep::formatMessageNoError("TRANSFER:", path + " -> " + newDir + dirInfo.fileName());
		}
	}


	return bRet;
}

void NugetStep::save(QXmlStreamWriter& a_writer)const
{
	a_writer.writeStartElement("NugetStep");
	a_writer.writeAttribute("Active", QString("%1").arg(isEnabled()));
	a_writer.writeAttribute("Version", QString("%1").arg(QString::fromLatin1(m_version)));
	a_writer.writeTextElement("Relative_path", QString::fromLatin1(m_relDir));
	a_writer.writeTextElement("Nuspecs", QString::fromLatin1(m_files));
	a_writer.writeEndElement();
}

void NugetStep::load(const QDomElement& a_reader)
{
	setEnable(a_reader.attribute("Active", "1") == "1");
	m_version = a_reader.attribute("Version").toStdString();
	auto nodeList = a_reader.childNodes();
	for (int i = 0; i < nodeList.count(); ++i)
	{
		auto elem = nodeList.at(i).toElement();
		if (!elem.isNull())
		{
			if (elem.tagName() == "Relative_path")
			{
				m_relDir = elem.text().toStdString();
			}
			else if (elem.tagName() == "Nuspecs")
			{
				m_files = elem.text().toStdString();
			}
		}
	}
}
