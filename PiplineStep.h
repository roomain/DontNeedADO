#pragma once
#include <QString>
#include <string>
#include <vector>

class QXmlStreamWriter;
class QDomElement;


struct Variable
{
	std::string m_varName;
	std::string m_varValue;
};

using VariableVector = std::vector<Variable>;

struct ExecuteArgs
{
	std::string workingDirectory;
	QString outputLog;
	VariableVector variables;
};



/*@brief base class for a pipline step*/
class PiplineStep
{
private:
	bool m_bEnable;

public:
	PiplineStep() : m_bEnable{ true } {}
	virtual bool execute(ExecuteArgs& a_args)const = 0;
	virtual void save(QXmlStreamWriter& a_writer)const = 0;
	virtual void load(const QDomElement& a_reader) = 0;
	virtual inline bool isTagStep()const { return false; }
	inline void setEnable(const bool a_enable) { m_bEnable = a_enable; }
	inline bool isEnabled()const noexcept { return m_bEnable; }

	static inline bool isVariable(const VariableVector& a_variables, const std::string& a_checkName, std::string& a_value)
	{
		for (const auto& var : a_variables)
		{
			if (var.m_varName == a_checkName)
			{
				a_value = var.m_varValue;
				return true;
			}
		}
		return false;
	}
};

/*
* Git Step: git clone <url>
* git submodule update --init --recursive
* Git Tag: git tag -a <tagname> -m <annotation>
* Git log: git log
* see https://blogs.sap.com/2018/06/22/generating-release-notes-from-git-commit-messages-using-basic-shell-commands-gitgrep/
*/