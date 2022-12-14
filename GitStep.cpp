#include "GitStep.h"
#include <QXmlStreamWriter>
#include <QDomElement>
#include <qstring.h>
#include <qfileinfo.h>


void GitStep::setUrl(const std::string& a_url)
{
	m_url = a_url;
}

std::string GitStep::url()const noexcept
{
	return m_url;
}

bool GitStep::execute(ExecuteArgs& a_args)const
{
	QString cmdGit = QString("git clone %2").arg(QString::fromLatin1(m_url));
	QProcess gitProcess;
	gitProcess.setWorkingDirectory(QString::fromLatin1(a_args.workingDirectory));
	gitProcess.start("cmd.exe" , QStringList() << "/c" << cmdGit);
	if (!gitProcess.waitForStarted())
	{
		a_args.outputLog += PiplineStep::formatMessage("GIT CLONE:", "NOT STARTED!", gitProcess);
		return false;
	}
	bool bOk = gitProcess.waitForFinished(-1);
	a_args.outputLog += PiplineStep::formatMessage("GIT CLONE:", gitProcess);
	//----------------------------------------------------------------------------------------------------------------------
	QFileInfo info(QString::fromLatin1(m_url));
	auto filename = info.fileName();
	filename = filename.left(filename.indexOf('.'));

	gitProcess.setWorkingDirectory(QString::fromLatin1(a_args.workingDirectory) + "/" + filename);
	gitProcess.start("cmd.exe", QStringList() << "/c" << "git submodule update --init --recursive");
	if (!gitProcess.waitForStarted())
	{
		a_args.outputLog += PiplineStep::formatMessage("GIT SUBMODULES:", "NOT STARTED!", gitProcess);
		return false;
	}
	bOk = gitProcess.waitForFinished(-1);

	a_args.outputLog += PiplineStep::formatMessage("GIT SUBMODULES:", gitProcess);
	return bOk;
}

void GitStep::save(QXmlStreamWriter& a_writer)const
{
	a_writer.writeStartElement("GitStep");
	a_writer.writeAttribute("Active", QString("%1").arg(isEnabled()));
	a_writer.writeTextElement("URL", QString::fromLatin1(m_url));
	a_writer.writeEndElement();
}

void GitStep::load(const QDomElement& a_reader)
{
	setEnable(a_reader.attribute("Active", "1") == "1");
	auto elem = a_reader.firstChildElement("URL");
	if (!elem.isNull())
	{
		m_url = elem.text().toStdString();
	}
}