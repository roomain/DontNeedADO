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

	std::string tagname;
	if (!PiplineStep::hasVariable(a_args.variables, m_tag, tagname))
		tagname = m_tag;

	QProcess gitProcess;
	gitProcess.setWorkingDirectory(QString::fromLatin1(a_args.workingDirectory + "/" + m_relDir));
	gitProcess.start("git", QStringList() << "tag" << "-a" << QString::fromLatin1(tagname) << "-m" << "\"" + QString::fromLatin1(m_comments) + "\"");
	if (!gitProcess.waitForStarted())
	{
		a_args.outputLog += PiplineStep::formatMessage("GIT TAG:", "NOT STARTED!", gitProcess);
		return false;
	}
	bool bOk = gitProcess.waitForFinished();

	a_args.outputLog += PiplineStep::formatMessage("GIT TAG:", gitProcess);

	//------------------------------------------------------------------------------------------------
	if (a_args.pushTag)
	{
		gitProcess.start("git", QStringList() << "push" << "--tags");
		if (!gitProcess.waitForStarted())
		{
			a_args.outputLog += PiplineStep::formatMessage("GIT PUSH TAG:", "NOT STARTED!", gitProcess);
			return false;
		}
		bOk = gitProcess.waitForFinished();

		a_args.outputLog += PiplineStep::formatMessage("GIT PUSH TAG:", gitProcess);
	}
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
