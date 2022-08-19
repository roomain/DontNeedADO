#pragma once
#include "PiplineStep.h"
#include <string>
#include <qstring.h>

class CMakeStep : public PiplineStep
{
private:
	enum CMAKE_OPT
	{
		WORKING_DIR = 0,
		OUT_DIR,
		COMPILER_VERS,
		PLATFORM,
		COMPILER_NAME,
		OPT_COUNT
	};
	std::string m_cmakeOpt[OPT_COUNT];

public:
	CMakeStep();
	void setWorkingDir(const std::string& a_value);
	void setOutDir(const std::string& a_value);
	void setCompilerVers(const std::string& a_value);
	void setPlatform(const std::string& a_value);
	void setCompilerName(const std::string& a_value);

	std::string workingDir()const noexcept;
	std::string outDir()const noexcept;
	std::string compilerVers()const noexcept;
	std::string platform()const noexcept;
	std::string compilerName()const noexcept;

	bool execute(ExecuteArgs& a_args)const final;
	void save(QXmlStreamWriter& a_writer)const final;
	void load(const QDomElement& a_reader) final;
	QString genCmd()const noexcept;
};

