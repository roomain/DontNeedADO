#include "TagStep.h"
#include <QXmlStreamWriter>
#include <QDomElement>
#include <qprocess.h>

void TagStep::setDir(const std::string& a_dir)
{
	m_relDir = a_dir;
}

void TagStep::setTag(const std::string& a_tag)
{
	m_tag = a_tag;
}

void TagStep::setComments(const std::string& a_comment)
{
	m_comments = a_comment;
}

std::string TagStep::relativeDir()const noexcept
{
	return m_relDir;
}

std::string TagStep::tag()const noexcept
{
	return m_tag;
}

std::string TagStep::comments()const noexcept
{
	return m_comments;
}

bool TagStep::execute(ExecuteArgs& a_args)const
{
	QString cmd = QString("git tag -a %1 -m \"%2\"").arg(QString::fromLatin1(m_tag)).arg(QString::fromLatin1(m_comments));
	QProcess gitProcess;
	gitProcess.setWorkingDirectory(QString::fromLatin1(a_args.workingDirectory + "/" + m_relDir));
	gitProcess.start("git", QStringList() << "tag" << "-a" << QString::fromLatin1(m_tag) << "-m" << "\"" + QString::fromLatin1(m_comments) + "\"");
	if (!gitProcess.waitForStarted())
	{
		a_args.outputLog += "\n\nGIT TAG:\nNOT STARTED!\n" + gitProcess.readAllStandardError();
		return false;
	}
	bool bOk = gitProcess.waitForFinished();

	auto lstArgs = gitProcess.arguments();
	QString argumentStr;
	std::for_each(lstArgs.begin(), lstArgs.end(), [&](const auto& arg) {argumentStr += arg + " "; });

	a_args.outputLog += "\n\nGIT TAG:\n" +
		gitProcess.program() + " " + argumentStr + "\n" +
		gitProcess.readAllStandardOutput() + "\n" + gitProcess.readAllStandardError();

	//------------------------------------------------------------------------------------------------

	gitProcess.start("git", QStringList() << "push" << "--tags");
	if (!gitProcess.waitForStarted())
	{
		a_args.outputLog += "\n\nGIT PUSH TAG:\nNOT STARTED!\n" + gitProcess.readAllStandardError();
		return false;
	}
	bOk = gitProcess.waitForFinished();

	lstArgs.clear();
	lstArgs = gitProcess.arguments();
	argumentStr.clear();
	std::for_each(lstArgs.begin(), lstArgs.end(), [&](const auto& arg) {argumentStr += arg + " "; });

	a_args.outputLog += "\n\nGIT PUSH TAG:\n" +
		gitProcess.program() + " " + argumentStr + "\n" +
		gitProcess.readAllStandardOutput() + "\n" + gitProcess.readAllStandardError();
	return bOk;
}

void TagStep::save(QXmlStreamWriter& a_writer)const
{
	a_writer.writeStartElement("TagStep");
	a_writer.writeAttribute("Active", QString("%1").arg(isEnabled()));
	a_writer.writeTextElement("Relative_directory", QString::fromLatin1(m_relDir));
	a_writer.writeTextElement("Tag", QString::fromLatin1(m_tag));
	a_writer.writeTextElement("Comment", QString::fromLatin1(m_comments));
	a_writer.writeEndElement();
}

void TagStep::load(const QDomElement& a_reader)
{
	setEnable(a_reader.attribute("Active", "1") == "1");
	auto nodeList = a_reader.childNodes();
	for (int i = 0; i < nodeList.count(); ++i)
	{
		auto elem = nodeList.at(i).toElement();
		if (!elem.isNull())
		{
			if (elem.tagName() == "Tag")
			{
				m_tag = elem.text().toStdString();
			}
			else if (elem.tagName() == "Comment")
			{
				m_comments = elem.text().toStdString();
			}
			else if (elem.tagName() == "Relative_directory")
			{
				m_relDir = elem.text().toStdString();
			}
		}
	}
}
