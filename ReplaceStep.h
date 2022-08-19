#pragma once
#include "PiplineStep.h"
#include <vector>


class ReplaceStep : public PiplineStep
{
public:
	struct TokenReplacement
	{
		std::string token;
		std::string filters;
		std::string value;
	};

private:
	std::vector<TokenReplacement> m_vTokens;
	bool m_bEnableVersion;
	std::string m_version;

public:
	ReplaceStep();
	void setVersion(const std::string& a_vers);
	std::string version()const noexcept;
	void enableVersion(const bool a_enable);
	bool versionEnable()const noexcept;
	void addToken(const std::string& a_token, const std::string& a_filters, const std::string& a_value);
	void removeToken(const int a_index);
	void replaceToken(const int a_index, const std::string& a_token, const std::string& a_filter, const std::string& a_value);
	bool execute(ExecuteArgs& a_args)const final;
	void save(QXmlStreamWriter& a_writer)const final;
	void load(const QDomElement& a_reader) final;

	using iterator = std::vector<TokenReplacement>::iterator;
	inline iterator begin() { return m_vTokens.begin(); }
	inline iterator end() { return m_vTokens.end(); }

	using const_iterator = std::vector<TokenReplacement>::const_iterator;
	inline const_iterator cbegin()const { return m_vTokens.cbegin(); }
	inline const_iterator cend()const { return m_vTokens.cend(); }
};

