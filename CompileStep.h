#pragma once
#include "PiplineStep.h"
#include <string>


class CompileStep : public PiplineStep
{
private:
	std::string m_relSLNPath;
	std::string m_conf;
	std::string m_platform;
	std::string m_compileFlags;
	
public:
	using PiplineStep::PiplineStep;

	void setRelativePath(const std::string& a_value);
	std::string relativePath()const noexcept;
	void setConfiguration(const std::string& a_value);
	std::string configuration()const noexcept;
	void setPlatform(const std::string& a_value);
	std::string platform()const noexcept;
	void setCompileFlags(const std::string& a_value);
	std::string conpileFlags()const noexcept;

	bool execute(ExecuteArgs& a_args)const final;
	void save(QXmlStreamWriter& a_writer)const final;
	void load(const QDomElement& a_reader) final;
};

