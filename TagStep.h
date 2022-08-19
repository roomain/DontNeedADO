#pragma once
#include "PiplineStep.h"

class TagStep : public PiplineStep
{
private:
	// TODO
public:
	using PiplineStep::PiplineStep;
	//TODO
	bool execute(ExecuteArgs& a_args)const final;
	void save(QXmlStreamWriter& a_writer)const final;
	void load(const QDomElement& a_reader) final;
	inline bool isTagStep()const final { return true; }
};

