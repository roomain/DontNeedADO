#pragma once
#include "PiplineStep.h"
class GitStep : public PiplineStep
{
private:
	std::string m_url;

public:
	using PiplineStep::PiplineStep;
	void setUrl(const std::string& a_url);
	std::string url()const noexcept;
	bool execute(ExecuteArgs& a_args)const final;
	void save(QXmlStreamWriter& a_writer)const final;
	void load(const QDomElement& a_reader) final;
};

