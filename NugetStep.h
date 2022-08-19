#pragma once
#include "PiplineStep.h"

class NugetStep : public PiplineStep
{
private:
	// TODO
public:
	using PiplineStep::PiplineStep;
	//TODO
	bool execute(ExecuteArgs& a_args)const final;
	void save(QXmlStreamWriter& a_writer)const final;
	void load(const QDomElement& a_reader) final;
};

