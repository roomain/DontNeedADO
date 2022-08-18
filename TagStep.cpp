#include "TagStep.h"



bool TagStep::execute(const ExecuteArgs& a_args)const
{
	bool bRet = false;
	//
	return bRet;
}

void TagStep::save(QXmlStreamWriter& a_writer)const
{
	a_writer.writeStartElement("CMakeStep");
	// TODO
	a_writer.writeEndElement();
}

void TagStep::load(QXmlStreamReader& a_reader)
{
	// TODO
}
