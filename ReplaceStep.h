#pragma once
#include "PiplineStep.h"
#include <vector>

class ReplaceStep : public PiplineStep
{
private:
	struct TokenReplacement
	{
		std::string token;
		std::string filter;
		std::string value;
	};

	std::vector<TokenReplacement> m_configuration;

public:
	using PiplineStep::PiplineStep;
	void addToken(const std::string& a_token, const std::string& a_filter, const std::string& a_value);
	void removeToken(const int a_index);
	void replaceToken(const int a_index, const std::string& a_token, const std::string& a_filter, const std::string& a_value);
	bool execute(const ExecuteArgs& a_args)const final;
	void save(QXmlStreamWriter& a_writer)const final;
	void load(QXmlStreamReader& a_reader) final;
};

