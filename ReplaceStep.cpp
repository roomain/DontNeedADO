#include "ReplaceStep.h"
#include <QXmlStreamWriter>
#include <QDomElement>
#include <qdiriterator.h>


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
	if(m_vTokens.size() > a_index)
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
	bool bRet = false;
	
	QDirIterator iter(QString::fromLatin1(a_args.workingDirectory), QDirIterator::Subdirectories);
	while (iter.hasNext())
	{
		for (const auto& token : m_vTokens)
		{
			//token.filters
			//
		}
	}
	
	return bRet;
}

void ReplaceStep::save(QXmlStreamWriter& a_writer)const
{
	a_writer.writeStartElement("ReplaceStep");
	a_writer.writeAttribute("Active", QString("%1").arg(isEnabled()));
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