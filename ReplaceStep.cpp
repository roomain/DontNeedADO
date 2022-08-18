#include "ReplaceStep.h"

void ReplaceStep::addToken(const std::string& a_token, const std::string& a_filter, const std::string& a_value)
{
	m_configuration.push_back(TokenReplacement{ a_token, a_filter, a_value });
}

void ReplaceStep::removeToken(const int a_index)
{
	m_configuration.erase(m_configuration.begin() + a_index);
}

void ReplaceStep::replaceToken(const int a_index, const std::string& a_token, const std::string& a_filters, const std::string& a_value)
{
	m_configuration[a_index].token = a_token;
	m_configuration[a_index].filters = a_filters;
	m_configuration[a_index].value = a_value;
}

bool ReplaceStep::execute(const ExecuteArgs& a_args)const
{
	bool bRet = false;
	//
	return bRet;
}

void ReplaceStep::save(QXmlStreamWriter& a_writer)const
{
	a_writer.writeStartElement("ReplaceStep");
	a_writer.writeAttribute("TokenCount", QString("%1").arg(m_configuration.size()));
	for (const auto& token : m_configuration)
	{
		a_writer.writeStartElement("ReplaceToken");
		a_writer.writeAttribute("Token", QString::fromLatin1(token.token));
		a_writer.writeAttribute("Filter", QString::fromLatin1(token.filters));
		a_writer.writeAttribute("Value", QString::fromLatin1(token.value));
		a_writer.writeEndElement();
	}
	a_writer.writeEndElement();
}

void ReplaceStep::load(QXmlStreamReader& a_reader)
{
	// TODO
}