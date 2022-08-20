#include "CompileStep.h"
#include <QXmlStreamWriter>
#include <QDomElement>
#include <QProcess>



void CompileStep::setRelativePath(const std::string& a_value)
{
	m_relSLNPath = a_value;
}

std::string CompileStep::relativePath()const noexcept
{
	return m_relSLNPath;
}

void CompileStep::setConfiguration(const std::string& a_value)
{
	m_conf = a_value;
}

std::string CompileStep::configuration()const noexcept
{
	return m_conf;
}

void CompileStep::setPlatform(const std::string& a_value)
{
	m_platform = a_value;
}

std::string CompileStep::platform()const noexcept
{
	return m_platform;
}

void CompileStep::setCompileFlags(const std::string& a_value)
{
	m_compileFlags = a_value;
}

std::string CompileStep::conpileFlags()const noexcept
{
	return m_compileFlags;
}

bool CompileStep::execute(ExecuteArgs& a_args)const
{
	bool bRet = false;
	QProcess buildProcess;
	buildProcess.setWorkingDirectory(QString::fromLatin1(a_args.workingDirectory));
	buildProcess.start("msbuild", QStringList() << QString("%1").arg(QString::fromLatin1(m_relSLNPath))
		<< QString("/Build ") << "/v:q" << "/nologo" << QString("/p:Configuration=%1").arg(QString::fromLatin1(m_conf))
		<< QString::fromLatin1(m_compileFlags));

	if (!buildProcess.waitForStarted())
	{
		a_args.outputLog += "\n\nBUILD:\nNOT STARTED!\n" + buildProcess.readAllStandardError();
		return false;
	}
	bRet = buildProcess.waitForFinished();
	a_args.outputLog += "\n\nBUILD:\n" + buildProcess.readAllStandardOutput() + "\n" + buildProcess.readAllStandardError();
	return bRet;
}

void CompileStep::save(QXmlStreamWriter& a_writer)const
{
	a_writer.writeStartElement("CompileStep");

	a_writer.writeAttribute("Active", QString("%1").arg(isEnabled()));
	a_writer.writeTextElement("Relative_path", QString::fromLatin1(m_relSLNPath));
	a_writer.writeTextElement("Configuration", QString::fromLatin1(m_conf));
	a_writer.writeTextElement("Platform", QString::fromLatin1(m_platform));
	a_writer.writeTextElement("Compile_flags", QString::fromLatin1(m_compileFlags));

	a_writer.writeEndElement();
}

void CompileStep::load(const QDomElement& a_reader)
{
	setEnable(a_reader.attribute("Active", "1") == "1");
	auto nodeList = a_reader.childNodes();
	for (int i = 0; i < nodeList.count(); ++i)
	{
		auto elem = nodeList.at(i).toElement();
		if (!elem.isNull())
		{
			if (elem.tagName() == "Relative_path")
			{
				m_relSLNPath = elem.text().toStdString();
			}
			else if (elem.tagName() == "Configuration")
			{
				m_conf = elem.text().toStdString();
			}
			else if (elem.tagName() == "Platform")
			{
				m_platform = elem.text().toStdString();
			}
			else if (elem.tagName() == "Compile_flags")
			{
				m_compileFlags = elem.text().toStdString();
			}
		}
	}
}
