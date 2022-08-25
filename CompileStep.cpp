#include "CompileStep.h"
#include <QXmlStreamWriter>
#include <QDomElement>



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

void CompileStep::setCompiler(const COMPILER a_compiler)
{
	m_compiler = a_compiler;
}

CompileStep::COMPILER CompileStep::compiler()const noexcept
{
	return m_compiler;
}

QString CompileStep::getCompilerPath()const noexcept
{
	QString sRet;
	switch (m_compiler)
	{
	case COMPILER::VS_2017:
		sRet = "\"C:/Program Files (x86)/Microsoft Visual Studio/2019/Professional/MSBuild/Current/Bin/msbuild.exe\"";
		break;

	case COMPILER::VS_2015:
		sRet = "\"C:/Program Files (x86)/Microsoft Visual Studio/2019/Professional/MSBuild/Current/Bin/msbuild.exe\"";
		break;

	case COMPILER::VS_2019:
		sRet = "\"C:/Program Files (x86)/Microsoft Visual Studio/2019/Professional/MSBuild/Current/Bin/msbuild.exe\"";
		break;

	default:
		break;
	}
	return sRet;
}
QString CompileStep::getCompilerVersion()const noexcept
{
	QString sRet;
	switch (m_compiler)
	{
	case COMPILER::VS_2015:
		sRet = "14.0";
		break;

	case COMPILER::VS_2017:
		sRet = "15.0";
		break;

	case COMPILER::VS_2019:
		sRet = "16.0";
		break;

	default:
		break;
	}
	return sRet;
}


bool CompileStep::execute(ExecuteArgs& a_args)const
{
	bool bRet = false;
	QProcess buildProcess;
	buildProcess.setWorkingDirectory(QString::fromLatin1(a_args.workingDirectory));

	buildProcess.start(getCompilerPath(), QStringList() << QString("%1").arg(QString::fromLatin1(m_relSLNPath))
		<< "/nologo" << "/nr:false" << "-maxcpucount" << QString("/p:platform=%1").arg(QString::fromLatin1(m_platform)) 
		<< QString("/p:Configuration=%1").arg(QString::fromLatin1(m_conf)) << QString("/p:VisualStudioVersion=%1").arg(getCompilerVersion())
		<< QString::fromLatin1(m_compileFlags));

	auto lstArgs = buildProcess.arguments();
	QString argumentStr;
	std::for_each(lstArgs.begin(), lstArgs.end(), [&](const auto& arg) {argumentStr += arg + " "; });
	QString sCmd = buildProcess.program() + " " + argumentStr;

	if (!buildProcess.waitForStarted())
	{
		a_args.outputLog += PiplineStep::formatMessage("BUILD:", "NOT STARTED!", buildProcess);
		return false;
	}
	bRet = buildProcess.waitForFinished();
	a_args.outputLog += PiplineStep::formatMessage("BUILD:", buildProcess);
	return bRet;
}

void CompileStep::save(QXmlStreamWriter& a_writer)const
{
	a_writer.writeStartElement("CompileStep");

	a_writer.writeAttribute("Active", QString("%1").arg(isEnabled()));
	a_writer.writeAttribute("Compiler", QString("%1").arg(static_cast<int>(m_compiler)));
	a_writer.writeTextElement("Relative_path", QString::fromLatin1(m_relSLNPath));
	a_writer.writeTextElement("Configuration", QString::fromLatin1(m_conf));
	a_writer.writeTextElement("Platform", QString::fromLatin1(m_platform));
	a_writer.writeTextElement("Compile_flags", QString::fromLatin1(m_compileFlags));

	a_writer.writeEndElement();
}

void CompileStep::load(const QDomElement& a_reader)
{
	setEnable(a_reader.attribute("Active", "1") == "1");
	m_compiler = static_cast<COMPILER>(a_reader.attribute("Compiler", 0).toInt());
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
