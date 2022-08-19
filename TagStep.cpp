#include "TagStep.h"
#include <QXmlStreamWriter>
#include <QDomElement>



bool TagStep::execute(ExecuteArgs& a_args)const
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

void TagStep::load(const QDomElement& a_reader)
{
	// TODO
}
