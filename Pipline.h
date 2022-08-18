#pragma once
#include <string>
#include <vector>
#include <memory>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

class PiplineStep;

class Pipline
{
private:
	std::string m_workingDir;
	std::vector<std::shared_ptr<PiplineStep>> m_vSteps;

public:
	Pipline() = default;
	int stepCount()const;
	void addStep(std::shared_ptr<PiplineStep>& a_step);
	void removeStep(const int a_index);
	void clear();
	void save(QXmlStreamWriter& a_writer)const;
	void load(QXmlStreamReader& a_reader);
	bool execute()const
};

