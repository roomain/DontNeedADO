#pragma once
#include <string>

struct ExecuteArgs
{
	bool executeWithoutTag;
	std::string workingDirectory;
	/*TODO*/
};

/*@brief base class for a pipline step*/
class PiplineStep
{
private:
	bool m_bEnable;

public:
	virtual bool execute(const ExecuteArgs& args) = 0;
};

/*
* Git Step: git clone <url>
* Git Tag: git tag -a <tagname> -m <annotation>
* Git log: git log
* see https://blogs.sap.com/2018/06/22/generating-release-notes-from-git-commit-messages-using-basic-shell-commands-gitgrep/
*/