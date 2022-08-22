#pragma once
#include "PiplineStep.h"
#include <string>


class CompileStep : public PiplineStep
{
public:
	enum class COMPILER : int
	{
		VS_2015 = 0,
		VS_2017,
		VS_2019
	};

private:
	std::string m_relSLNPath;
	std::string m_conf;
	std::string m_platform;
	std::string m_compileFlags;
	COMPILER m_compiler = COMPILER::VS_2015;

	QString getCompilerPath()const noexcept;
	QString getCompilerVersion()const noexcept;
	
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
	void setCompiler(const COMPILER a_compiler);
	COMPILER compiler()const noexcept;

	bool execute(ExecuteArgs& a_args)const final;
	void save(QXmlStreamWriter& a_writer)const final;
	void load(const QDomElement& a_reader) final;
};

