#include "CMakeStep.h"
#include <qprocess.h>
#include <QXmlStreamWriter>
#include <QDomElement>


CMakeStep::CMakeStep()
{
	m_cmakeOpt[WORKING_DIR];
	m_cmakeOpt[OUT_DIR] = "VS2019";
	m_cmakeOpt[COMPILER_VERS] = "v142";
	m_cmakeOpt[PLATFORM] = "x64";
	m_cmakeOpt[COMPILER_NAME] = "Visual Studio 16 2019";
}

void CMakeStep::setWorkingDir(const std::string& a_value)
{
	m_cmakeOpt[WORKING_DIR] = a_value;
}

void CMakeStep::setOutDir(const std::string& a_value)
{
	m_cmakeOpt[OUT_DIR] = a_value;
}

void CMakeStep::setCompilerVers(const std::string& a_value)
{
	m_cmakeOpt[COMPILER_VERS] = a_value;
}

void CMakeStep::setPlatform(const std::string& a_value)
{
	m_cmakeOpt[PLATFORM] = a_value;
}

void CMakeStep::setCompilerName(const std::string& a_value)
{
	m_cmakeOpt[COMPILER_NAME] = a_value;
}


std::string CMakeStep::workingDir()const noexcept
{
	return m_cmakeOpt[WORKING_DIR];
}

std::string CMakeStep::outDir()const noexcept
{
	return m_cmakeOpt[OUT_DIR];
}

std::string CMakeStep::compilerVers()const noexcept
{
	return m_cmakeOpt[COMPILER_VERS];
}

std::string CMakeStep::platform()const noexcept
{
	return m_cmakeOpt[PLATFORM];
}

std::string CMakeStep::compilerName()const noexcept
{
	return m_cmakeOpt[COMPILER_NAME];
}

QString CMakeStep::genCmd()const noexcept
{
	return QString("\"C:/Program Files/CMake/bin/CMake\" -B \"%1\" -T %2 -A %3 -G \"%4\"").arg(QString::fromLatin1(m_cmakeOpt[OUT_DIR]))
		.arg(QString::fromLatin1(m_cmakeOpt[COMPILER_VERS]))
		.arg(QString::fromLatin1(m_cmakeOpt[PLATFORM]))
		.arg(QString::fromLatin1(m_cmakeOpt[COMPILER_NAME]));
}

bool CMakeStep::execute(ExecuteArgs& a_args)const
{
	bool bRet = false;
	QProcess cmakeProcess;

	cmakeProcess.setWorkingDirectory(QString::fromLatin1(a_args.workingDirectory + "/" + m_cmakeOpt[WORKING_DIR]));

	cmakeProcess.start("C:/Program Files/CMake/bin/CMake", QStringList() << QString("-B %1").arg(QString::fromLatin1(m_cmakeOpt[OUT_DIR]))
	<< QString("-T %1").arg(QString::fromLatin1(m_cmakeOpt[COMPILER_VERS])) 
		<< QString("-A %1").arg(QString::fromLatin1(m_cmakeOpt[PLATFORM]))
		<< QString("-G %1").arg(QString::fromLatin1(m_cmakeOpt[COMPILER_NAME])));
	if (!cmakeProcess.waitForStarted())
	{
		a_args.outputLog += "\n\nCMAKE:\nNOT STARTED!\n" + cmakeProcess.readAllStandardError();
		return false;
	}
	bRet = cmakeProcess.waitForFinished();

	auto lstArgs = cmakeProcess.arguments();
	QString argumentStr;
	std::for_each(lstArgs.begin(), lstArgs.end(), [&](const auto& arg) {argumentStr += arg + " "; });

	a_args.outputLog += "\n\nCMAKE:" + 
		cmakeProcess.program() + " " + argumentStr + "\n" +
		cmakeProcess.readAllStandardOutput() + "\n" + cmakeProcess.readAllStandardError();
	return bRet;
}

void CMakeStep::save(QXmlStreamWriter& a_writer)const
{
	a_writer.writeStartElement("CMakeStep");
	a_writer.writeAttribute("Active", QString("%1").arg(isEnabled()));
	a_writer.writeTextElement("Working_directory",QString::fromLatin1(m_cmakeOpt[WORKING_DIR]));
	a_writer.writeTextElement("Output_directory", QString::fromLatin1(m_cmakeOpt[OUT_DIR]));
	a_writer.writeTextElement("Compiler_vers", QString::fromLatin1(m_cmakeOpt[COMPILER_VERS]));
	a_writer.writeTextElement("Platform", QString::fromLatin1(m_cmakeOpt[PLATFORM]));
	a_writer.writeTextElement("Compiler_name", QString::fromLatin1(m_cmakeOpt[COMPILER_NAME]));
	a_writer.writeEndElement();
}

void CMakeStep::load(const QDomElement& a_reader)
{
	setEnable(a_reader.attribute("Active", "1") == "1");
	auto nodeList = a_reader.childNodes();
	for (int i = 0; i < nodeList.count(); ++i)
	{
		auto elem = nodeList.at(i).toElement();
		if (!elem.isNull())
		{
			if (elem.tagName() == "Working_directory")
			{
				m_cmakeOpt[WORKING_DIR] = elem.text().toStdString();
			}
			else if (elem.tagName() == "Output_directory")
			{
				m_cmakeOpt[OUT_DIR] = elem.text().toStdString();
			}
			else if (elem.tagName() == "Compiler_vers")
			{
				m_cmakeOpt[COMPILER_VERS] = elem.text().toStdString();
			}
			else if (elem.tagName() == "Platform")
			{
				m_cmakeOpt[PLATFORM] = elem.text().toStdString();
			}
			else if (elem.tagName() == "Compiler_name")
			{
				m_cmakeOpt[COMPILER_NAME] = elem.text().toStdString();
			}
		}
	}
}
