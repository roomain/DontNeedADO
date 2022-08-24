#pragma once
#include "PiplineStep.h"
#include <string>
#include <vector>

class VariablesStep : public PiplineStep
{
private:
	VariableVector m_vVariables;

public:
	using PiplineStep::PiplineStep;
	void addVariable(const std::string& a_name = "", const std::string& a_value = "");
	void deleteVariable(const int a_index);
	void updateVariable(const int a_index, const std::string& a_name, const std::string& a_value);
	bool execute(ExecuteArgs& a_args)const final;
	void save(QXmlStreamWriter& a_writer)const final;
	void load(const QDomElement& a_reader) final;


	using iterator = VariableVector::iterator;
	inline iterator begin() { return m_vVariables.begin(); }
	inline iterator end() { return m_vVariables.end(); }

	using const_iterator = VariableVector::const_iterator;
	inline const_iterator cbegin()const { return m_vVariables.cbegin(); }
	inline const_iterator cend()const { return m_vVariables.cend(); }
};

