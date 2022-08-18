#include "GitStep.h"



bool GitStep::execute(const ExecuteArgs& a_args)const
{
	bool bRet = false;
	//
	return bRet;
}

void GitStep::save(QXmlStreamWriter& a_writer)const
{
	a_writer.writeStartElement("GitStep");
	// TODO
	a_writer.writeEndElement();
}

void GitStep::load(QXmlStreamReader& a_reader)
{
	// TODO
}