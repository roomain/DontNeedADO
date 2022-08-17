#pragma once
#include <string>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

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
	PiplineStep() : m_bEnable{ true } {}
	virtual bool execute(const ExecuteArgs& a_args)const = 0;
	virtual void save(QXmlStreamWriter& a_writer)const = 0;
	virtual void load(QXmlStreamReader& a_reader) = 0;
};

/*
* Git Step: git clone <url>
* Git Tag: git tag -a <tagname> -m <annotation>
* Git log: git log
* see https://blogs.sap.com/2018/06/22/generating-release-notes-from-git-commit-messages-using-basic-shell-commands-gitgrep/
*/