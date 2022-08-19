#include "NugetStep.h"
#include <QXmlStreamWriter>
#include <QDomElement>



bool NugetStep::execute(ExecuteArgs& a_args)const
{
	bool bRet = false;
	//
	return bRet;
}

void NugetStep::save(QXmlStreamWriter& a_writer)const
{
	a_writer.writeStartElement("CMakeStep");
	// TODO
	a_writer.writeEndElement();
}

void NugetStep::load(const QDomElement& a_reader)
{
	// TODO
}
