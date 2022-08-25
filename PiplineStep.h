#pragma once
#include <QString>
#include <string>
#include <vector>
#include <QProcess>

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

	static bool isVariable(const VariableVector& a_variables, const std::string& a_checkName, std::string& a_value);
	static QString toHtmlMessage(const QString& a_message, bool a_bold = false);
	static QString toHtmlSuccess(const QString& a_message, bool a_bold = false);
	static QString toHtmlError(const QString& a_message, bool a_bold = false);
	static QString formatCommand(const QProcess& a_process);
	static QString formatMessage(const QString& a_stepName, const QString& a_customError, QProcess& a_process);
	static QString formatMessage(const QString& a_stepName, QProcess& a_process);
	static QString formatMessage(const QString& a_stepName, const QString& a_customError);
	static QString formatMessageNoError(const QString& a_stepName, const QString& a_messages);
};

/*
* Git Step: git clone <url>
* git submodule update --init --recursive
* Git Tag: git tag -a <tagname> -m <annotation>
* Git log: git log
* see https://blogs.sap.com/2018/06/22/generating-release-notes-from-git-commit-messages-using-basic-shell-commands-gitgrep/
*/