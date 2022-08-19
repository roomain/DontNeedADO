#include "CompileStep.h"
#include <QXmlStreamWriter>
#include <QDomElement>



bool CompileStep::execute(ExecuteArgs& a_args)const
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

void CompileStep::load(const QDomElement& a_reader)
{
	// TODO
}
