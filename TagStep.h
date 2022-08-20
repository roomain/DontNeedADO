#pragma once
#include "PiplineStep.h"
#include <string>

class TagStep : public PiplineStep
{
private:
	std::string m_tag;
	std::string m_comments;
	std::string m_relDir;

public:
	using PiplineStep::PiplineStep;
	void setDir(const std::string& a_dir);
	void setTag(const std::string& a_tag);
	void setComments(const std::string& a_comment);
	std::string relativeDir()const noexcept;
	std::string tag()const noexcept;
	std::string comments()const noexcept;
	bool execute(ExecuteArgs& a_args)const final;
	void save(QXmlStreamWriter& a_writer)const final;
	void load(const QDomElement& a_reader) final;
	inline bool isTagStep()const final { return true; }
};

