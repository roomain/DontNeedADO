#pragma once
#include "PiplineStep.h"


class CompileStep : public PiplineStep
{
private:
	// TODO
public:
	using PiplineStep::PiplineStep;
	//TODO
	bool execute(const ExecuteArgs& a_args)const final;
	void save(QXmlStreamWriter& a_writer)const final;
	void load(QXmlStreamReader& a_reader) final;
};

