#pragma once
#include <string>
#include <vector>
#include <memory>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include "PiplineStep.h"
#include <thread>

class Pipline : public QObject
{
	Q_OBJECT

private:
	std::string m_workingDir;
	std::vector<std::shared_ptr<PiplineStep>> m_vSteps;
	std::unique_ptr<std::thread> m_worker;

public:
	Pipline() = default;
	~Pipline();
	int stepCount()const;
	void setWorkingDir(const std::string& a_dir);
	std::string workingDir()const noexcept;
	void addStep(const std::shared_ptr<PiplineStep>& a_step);
	void removeStep(const int a_index);
	void clear();
	void save(QXmlStreamWriter& a_writer)const;
	void load(const QDomElement& a_reader);
	void execute(const bool a_dontUseTag);
	void moveStep(const int a_src, const int a_dst);

	using iterator = std::vector<std::shared_ptr<PiplineStep>>::iterator;
	iterator begin() { return m_vSteps.begin(); }
	iterator end() { return m_vSteps.end(); }

	using const_iterator = std::vector<std::shared_ptr<PiplineStep>>::const_iterator;
	const_iterator cbegin()const { return m_vSteps.cbegin(); }
	const_iterator cend()const { return m_vSteps.cend(); }

	std::shared_ptr<PiplineStep> operator[](const int a_index)
	{
		return m_vSteps[a_index];
	}

signals:
	void sg_update(int, const QString&);
	void sg_finished(bool);
	void sg_start(int);
};

