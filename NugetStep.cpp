#include "NugetStep.h"



bool NugetStep::execute(const ExecuteArgs& a_args)const
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

void NugetStep::load(QXmlStreamReader& a_reader)
{
	// TODO
}
