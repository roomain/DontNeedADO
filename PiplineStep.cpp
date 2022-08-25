#include "PiplineStep.h"

bool PiplineStep::hasVariable(const VariableVector& a_variables, const std::string& a_checkName, std::string& a_value)
{
	bool bRet = false;
	QString temp = QString::fromLatin1(a_checkName);
	for (const auto& var : a_variables)
	{
		temp.replace(QString::fromLatin1(var.m_varName), QString::fromLatin1(var.m_varValue));
	}
	a_value = temp.toStdString();
	return true;
}

bool PiplineStep::isVariable(const VariableVector& a_variables, const std::string& a_checkName, std::string& a_value)
{
	for (const auto& var : a_variables)
	{
		if (QString::fromLatin1(var.m_varName).compare(QString::fromLatin1(a_checkName),Qt::CaseInsensitive) == 0)
		{
			a_value = var.m_varValue;
			return true;
		}
	}
	return false;
}

QString PiplineStep::formatCommand(const QProcess& a_process)
{
	auto lstArgs = a_process.arguments();
	QString argumentStr;
	std::for_each(lstArgs.begin(), lstArgs.end(), [&](const auto& arg) {argumentStr += arg + " "; });
	return a_process.program() + " " + argumentStr;
}

QString PiplineStep::formatMessage(const QString& a_stepName, const QString& a_customError, QProcess& a_process)
{
	QString sCommand = PiplineStep::formatCommand(a_process);
	return PiplineStep::toHtmlMessage(a_stepName, true) + "<br>" + PiplineStep::toHtmlMessage(sCommand) + "<br>" +
		PiplineStep::toHtmlError(a_customError) + "<br>" + 
		PiplineStep::toHtmlError(QString::fromUtf8(a_process.readAllStandardError())) + "<br>";
}

QString PiplineStep::formatMessage(const QString& a_stepName, QProcess& a_process)
{
	QString sCommand = PiplineStep::formatCommand(a_process);
	return PiplineStep::toHtmlMessage(a_stepName, true) + "<br>" + PiplineStep::toHtmlMessage(sCommand) + "<br>" +
		PiplineStep::toHtmlMessage(QString::fromUtf8(a_process.readAllStandardOutput())) + "<br>" +
		PiplineStep::toHtmlError(QString::fromUtf8(a_process.readAllStandardError())) + "<br>";
}

QString PiplineStep::formatMessage(const QString& a_stepName, const QString& a_customError)
{
	return PiplineStep::toHtmlMessage(a_stepName, true) + "<br>" + PiplineStep::toHtmlError(a_customError) + "<br>";
}

QString PiplineStep::formatMessageNoError(const QString& a_stepName, const QString& a_messages)
{
	return PiplineStep::toHtmlMessage(a_stepName) + " " + PiplineStep::toHtmlSuccess(a_messages) + "<br>";
}

QString PiplineStep::toHtmlMessage(const QString& a_message, bool a_bold)
{
	QString temp = a_message;
	if(a_bold)
		return "<span style=\"color:#FFFFFF;\"><b>" + temp.replace('\n', "<br>") + "</b></span>";
	return "<span style=\"color:#FFFFFF;\">" + temp.replace('\n', "<br>") + "</span>";
}

QString PiplineStep::toHtmlSuccess(const QString& a_message, bool a_bold)
{
	QString temp = a_message;
	if (a_bold)
		return "<span style=\"color:#00FF00;\"><b>" + temp.replace('\n', "<br>") + "</b></span>";
	return "<span style=\"color:#00FF00;\">" + temp.replace('\n', "<br>") + "</span>";
}

QString PiplineStep::toHtmlError(const QString& a_message, bool a_bold)
{
	QString temp = a_message;
	if (a_bold)
		return "<span style=\"color:#FF0000;\"><b>" + temp.replace('\n', "<br>") + "</b></span>";
	return "<span style=\"color:#FF0000;\">" + temp.replace('\n', "<br>") + "</span>";
}