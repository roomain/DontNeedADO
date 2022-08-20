#pragma once
#include "PiplineStep.h"
#include <string>

class NugetStep : public PiplineStep
{
private:
	std::string m_relDir;
	std::string m_files;

public:
	using PiplineStep::PiplineStep;
	void setRelativePath(const std::string& a_value);
	std::string relativePath()const noexcept;
	void setFiles(const std::string& a_value);
	std::string files()const noexcept;
	bool execute(ExecuteArgs& a_args)const final;
	void save(QXmlStreamWriter& a_writer)const final;
	void load(const QDomElement& a_reader) final;
};

