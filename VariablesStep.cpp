#include "VariablesStep.h"
#include <QXmlStreamWriter>
#include <QDomElement>


void VariablesStep::addVariable(const std::string& a_name, const std::string& a_value)
{
	m_vVariables.push_back(Variable{ a_name, a_value });
}

void VariablesStep::deleteVariable(const int a_index)
{
	m_vVariables.erase(m_vVariables.begin() + a_index);
}

void VariablesStep::updateVariable(const int a_index, const std::string& a_name, const std::string& a_value)
{
	m_vVariables[a_index].m_varName = a_name;
	m_vVariables[a_index].m_varValue = a_value;
}

bool VariablesStep::execute(ExecuteArgs& a_args)const
{
	a_args.outputLog += PiplineStep::formatMessage("SET VARIABLES:", "");
	for (const auto& var : m_vVariables)
	{
		a_args.outputLog += PiplineStep::formatMessageNoError("SET:", QString::fromLatin1(var.m_varName) + " = " + 
			QString::fromLatin1(var.m_varValue));
		a_args.variables.push_back(var);
	}
	a_args.outputLog += PiplineStep::formatMessage("", "");
	return true;
}

void VariablesStep::save(QXmlStreamWriter& a_writer)const
{
	a_writer.writeStartElement("VariablesStep");
	a_writer.writeAttribute("Active", QString("%1").arg(isEnabled()));
	a_writer.writeAttribute("VariableCount", QString("%1").arg(m_vVariables.size()));
	for (const auto& var : m_vVariables)
	{
		a_writer.writeStartElement("Variable");
		a_writer.writeAttribute("Name", QString::fromLatin1(var.m_varName));
		a_writer.writeAttribute("Value", QString::fromLatin1(var.m_varValue));
		a_writer.writeEndElement();
	}
	a_writer.writeEndElement();
}

void VariablesStep::load(const QDomElement& a_reader)
{
	setEnable(a_reader.attribute("Active", "1") == "1");
	auto nodeList = a_reader.childNodes();
	for (int i = 0; i < nodeList.count(); ++i)
	{
		auto elem = nodeList.at(i).toElement();
		if (!elem.isNull())
		{
			m_vVariables.push_back(Variable{ elem.attribute("Name", "").toStdString(), elem.attribute("Value", "").toStdString() });
		}
	}
}