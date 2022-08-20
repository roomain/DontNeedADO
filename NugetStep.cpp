#include "NugetStep.h"
#include <QXmlStreamWriter>
#include <QDomElement>


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
	//
	return bRet;
}

void NugetStep::save(QXmlStreamWriter& a_writer)const
{
	a_writer.writeStartElement("NugetStep");
	a_writer.writeAttribute("Active", QString("%1").arg(isEnabled()));
	a_writer.writeTextElement("Relative_path", QString::fromLatin1(m_relDir));
	a_writer.writeTextElement("Nuspecs", QString::fromLatin1(m_files));
	a_writer.writeEndElement();
}

void NugetStep::load(const QDomElement& a_reader)
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
				m_relDir = elem.text().toStdString();
			}
			else if (elem.tagName() == "Nuspecs")
			{
				m_files = elem.text().toStdString();
			}
		}
	}
}
