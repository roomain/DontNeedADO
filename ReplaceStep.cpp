#include "ReplaceStep.h"
#include <QXmlStreamWriter>
#include <QDomElement>
#include <qdiriterator.h>
#include <qtextstream.h>


ReplaceStep::ReplaceStep() : PiplineStep(), m_bEnableVersion{ false }
{}

void ReplaceStep::setVersion(const std::string& a_vers)
{
	m_version = a_vers;
}

std::string ReplaceStep::version()const noexcept
{
	return m_version;
}

void ReplaceStep::enableVersion(const bool a_enable)
{
	m_bEnableVersion = a_enable;
}

bool ReplaceStep::versionEnable()const noexcept
{
	return m_bEnableVersion;
}

void ReplaceStep::addToken(const std::string& a_token, const std::string& a_filter, const std::string& a_value)
{
	m_vTokens.push_back(TokenReplacement{ a_token, a_filter, a_value });
}

void ReplaceStep::removeToken(const int a_index)
{
	if (m_vTokens.size() > a_index)
		m_vTokens.erase(m_vTokens.begin() + a_index);
}

void ReplaceStep::replaceToken(const int a_index, const std::string& a_token, const std::string& a_filters, const std::string& a_value)
{
	m_vTokens[a_index].token = a_token;
	m_vTokens[a_index].filters = a_filters;
	m_vTokens[a_index].value = a_value;
}


bool ReplaceStep::execute(ExecuteArgs& a_args)const
{
	bool bRet = true;
	a_args.outputLog += PiplineStep::formatMessage("REPLACE IN FILES:", "");
	for (const auto& token : m_vTokens)
	{
		QString filter = QString::fromLatin1(token.filters);
		auto lstFilters = filter.split(';');
		std::string tokenValue;
		if (!PiplineStep::isVariable(a_args.variables, token.token, tokenValue))
			tokenValue = token.value;

		QDirIterator iter(QString::fromLatin1(a_args.workingDirectory), lstFilters,
			QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
		while (iter.hasNext())
		{
			QString path = iter.next();
			QFileInfo dirInfo(path);
			if (dirInfo.isFile())
			{
				QFile file(path);
				if (file.open(QIODevice::ReadWrite))
				{
					QByteArray data = file.readAll();
					data.replace(QByteArray(token.token.c_str()), QByteArray(tokenValue.c_str()));
					file.reset();
					file.resize(0);
					file.write(data);
					file.flush();
					file.close();
					a_args.outputLog += PiplineStep::formatMessageNoError("REPLACE:", path + " : " + QString::fromLatin1(token.token) + " -> " + 
						QString::fromLatin1(tokenValue));
				}
			}
		}
	}

	a_args.outputLog += PiplineStep::formatMessage("", "");
	if (m_bEnableVersion && !m_version.empty())
	{
		std::string version;
		if (!PiplineStep::isVariable(a_args.variables, m_version, version))
			version = m_version;

		QStringList versionSplitted = QString::fromLatin1(version).split('.');

		a_args.outputLog += PiplineStep::formatMessage("VERSIONING RC FILES:", "");
		QDirIterator iter(QString::fromLatin1(a_args.workingDirectory), QStringList() << "*.rc",
			QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
		while (iter.hasNext())
		{
			QString path = iter.next();
			QFileInfo dirInfo(path);
			if (dirInfo.isFile())
			{
				QFile file(path);
				if (file.open(QIODevice::ReadWrite))
				{
					QTextStream stream(&file);
					QByteArray data;
					QString line;
					while (!stream.atEnd())
					{
						line = stream.readLine();
						if (line.indexOf("FILEVERSION") >= 0)
						{
							line = line.left(line.indexOf("FILEVERSION"));
							line += "FILEVERSION ";
							for (int i = 0; i < versionSplitted.size() - 1; ++i)
							{
								line += versionSplitted[i] + ", ";
							}
							line += versionSplitted.last();
						}
						else if (line.indexOf("PRODUCTVERSION") >= 0)
						{
							line = line.left(line.indexOf("PRODUCTVERSION"));
							line += "PRODUCTVERSION ";
							for (int i = 0; i < versionSplitted.size() - 1; ++i)
							{
								line += versionSplitted[i] + ", ";
							}
							line += versionSplitted.last();
						}
						else if (line.contains("VALUE \"FileVersion\""))
						{
							int iIndex = line.indexOf("VALUE");
							line = line.left(iIndex);
							line += "VALUE \"FileVersion\", \"" + QString::fromLatin1(version) + "\"";
						}
						else if (line.contains("VALUE \"ProductVersion\""))
						{
							int iIndex = line.indexOf("VALUE");
							line = line.left(iIndex);
							line += "VALUE \"ProductVersion\", \"" + QString::fromLatin1(version) + "\"";
						}
						line += "\n";
						data.append(line.toStdString().c_str());
					}

					file.reset();
					file.resize(0);
					file.write(data);
					file.close();

					a_args.outputLog += PiplineStep::formatMessageNoError("VERSIONING:", path + " : " + QString::fromLatin1(version));
				}
			}
		}


		a_args.outputLog += PiplineStep::formatMessage("", "");
		a_args.outputLog += PiplineStep::formatMessage("VERSIONING ASSEMBLY INFO FILES:", "");
		QDirIterator iterCS(QString::fromLatin1(a_args.workingDirectory), QStringList() << "AssemblyInfo.cs",
			QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
		while (iterCS.hasNext())
		{
			QString path = iterCS.next();
			QFileInfo dirInfo(path);
			if (dirInfo.isFile())
			{
				QFile file(path);
				if (file.open(QIODevice::ReadWrite))
				{
					QTextStream stream(&file);
					QByteArray data;
					QString line;
					while (!stream.atEnd())
					{
						line = stream.readLine();
						if (line.startsWith("[assembly: AssemblyVersion"))
						{
							line = "[assembly: AssemblyVersion(\""+ QString::fromLatin1(version) +"\")]";
						}
						else if (line.startsWith("[assembly: AssemblyFileVersion"))
						{
							line = "[assembly: AssemblyFileVersion(\"" + QString::fromLatin1(version) + "\")]";
						}
						line += "\n";
						data.append(line.toStdString().c_str());
					}

					file.reset();
					file.resize(0);
					file.write(data);
					file.close();
					a_args.outputLog += PiplineStep::formatMessageNoError("VERSIONING:", path + " : " + QString::fromLatin1(version));
				}
			}
		}
		a_args.outputLog += PiplineStep::formatMessage("", "");
	}

	return bRet;
}

void ReplaceStep::save(QXmlStreamWriter& a_writer)const
{
	a_writer.writeStartElement("ReplaceStep");
	a_writer.writeAttribute("Active", QString("%1").arg(isEnabled()));
	a_writer.writeAttribute("Version", QString("%1").arg(QString::fromLatin1(m_version)));
	a_writer.writeAttribute("Version_dll", QString("%1").arg(m_bEnableVersion));
	a_writer.writeAttribute("TokenCount", QString("%1").arg(m_vTokens.size()));
	for (const auto& token : m_vTokens)
	{
		a_writer.writeStartElement("ReplaceToken");
		a_writer.writeAttribute("Token", QString::fromLatin1(token.token));
		a_writer.writeAttribute("Filter", QString::fromLatin1(token.filters));
		a_writer.writeAttribute("Value", QString::fromLatin1(token.value));
		a_writer.writeEndElement();
	}
	a_writer.writeEndElement();
}

void ReplaceStep::load(const QDomElement& a_reader)
{
	setEnable(a_reader.attribute("Active", "1") == "1");
	m_bEnableVersion = a_reader.attribute("Version_dll", "1") == "1";
	m_version = a_reader.attribute("Version").toStdString();
	auto nodeList = a_reader.childNodes();
	for (int i = 0; i < nodeList.count(); ++i)
	{
		auto elem = nodeList.at(i).toElement();
		if (!elem.isNull())
		{
			TokenReplacement token;
			token.token = elem.attribute("Token", "").toStdString();
			token.filters = elem.attribute("Filter", "").toStdString();
			token.value = elem.attribute("Value", "").toStdString();
			m_vTokens.push_back(token);
		}
	}
}