#include "CMakeStep.h"



bool CMakeStep::execute(const ExecuteArgs& a_args)const
{
	bool bRet = false;
	//
	return bRet;
}

void CMakeStep::save(QXmlStreamWriter& a_writer)const
{
	a_writer.writeStartElement("CMakeStep");
	// TODO
	a_writer.writeEndElement();
}

void CMakeStep::load(QXmlStreamReader& a_reader)
{
	// TODO
}
