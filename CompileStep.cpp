#include "CompileStep.h"



bool CompileStep::execute(const ExecuteArgs& a_args)const
{
	bool bRet = false;
	//
	return bRet;
}

void CompileStep::save(QXmlStreamWriter& a_writer)const
{
	a_writer.writeStartElement("CMakeStep");
	// TODO
	a_writer.writeEndElement();
}

void CompileStep::load(QXmlStreamReader& a_reader)
{
	// TODO
}
