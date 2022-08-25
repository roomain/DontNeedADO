#include "Pipline.h"
#include "GitStep.h"
#include "CMakeStep.h"
#include "ReplaceStep.h"
#include "CompileStep.h"
#include "NugetStep.h"
#include "TagStep.h"
#include "VariablesStep.h"
#include <QXmlStreamWriter>
#include <QDomElement>


Pipline::~Pipline()
{
	if (m_worker)
		m_worker->join();
}

int Pipline::stepCount()const
{
	return static_cast<int>(m_vSteps.size());
}

void Pipline::moveStep(const int a_src, const int a_dst)
{
	auto pStep = m_vSteps[a_src];
	int iDest = a_dst < a_src ? a_dst : a_dst - 1;
	m_vSteps.erase(m_vSteps.begin() + a_src);
	m_vSteps.insert(m_vSteps.begin() + iDest, pStep);
}

void Pipline::addStep(const std::shared_ptr<PiplineStep>& a_step)
{
	m_vSteps.push_back(a_step);
}

void Pipline::setWorkingDir(const std::string& a_dir)
{
	m_workingDir = a_dir;
}

std::string Pipline::workingDir()const noexcept
{
	return m_workingDir;
}

void Pipline::removeStep(const int a_index)
{
	m_vSteps.erase(m_vSteps.begin() + a_index);
}

void Pipline::clear()
{
	m_vSteps.clear();
}

void Pipline::save(QXmlStreamWriter& a_writer)const
{
	a_writer.writeStartElement("Pipline");
	for (auto& step : m_vSteps)
		step->save(a_writer);
	a_writer.writeEndElement();
}

void Pipline::load(const QDomElement& a_reader)
{
	auto nodeList = a_reader.childNodes();
	for (int i = 0; i < nodeList.count(); ++i)
	{
		auto elem = nodeList.at(i).toElement();
		if (!elem.isNull())
		{
			if (elem.tagName() == "GitStep")
			{
				std::shared_ptr<GitStep> pStep = std::make_shared<GitStep>();
				pStep->load(elem);
				m_vSteps.push_back(pStep);
			}
			else if (elem.tagName() == "CMakeStep")
			{
				std::shared_ptr<CMakeStep> pStep = std::make_shared<CMakeStep>();
				pStep->load(elem);
				m_vSteps.push_back(pStep);
			}
			else if (elem.tagName() == "ReplaceStep")
			{
				std::shared_ptr<ReplaceStep> pStep = std::make_shared<ReplaceStep>();
				pStep->load(elem);
				m_vSteps.push_back(pStep);
			}
			else if (elem.tagName() == "CompileStep")
			{
				std::shared_ptr<CompileStep> pStep = std::make_shared<CompileStep>();
				pStep->load(elem);
				m_vSteps.push_back(pStep);
			}
			else if (elem.tagName() == "TagStep")
			{
				std::shared_ptr<TagStep> pStep = std::make_shared<TagStep>();
				pStep->load(elem);
				m_vSteps.push_back(pStep);
			}
			else if (elem.tagName() == "NugetStep")
			{
				std::shared_ptr<NugetStep> pStep = std::make_shared<NugetStep>();
				pStep->load(elem);
				m_vSteps.push_back(pStep);
			}
			else if (elem.tagName() == "VariablesStep")
			{
				std::shared_ptr<VariablesStep> pStep = std::make_shared<VariablesStep>();
				pStep->load(elem);
				m_vSteps.push_back(pStep);
			}
		}
	}
}

void Pipline::execute(const bool a_dontUseTag)
{
	bool bRet = false;
	if (m_vSteps.size() > 0)
	{
		if (m_worker)
			m_worker->join();

		m_worker = std::make_unique<std::thread>([=]()
			{
				ExecuteArgs args{ .workingDirectory = this->m_workingDir };
				int iIndex = 0;
				emit this->sg_start(m_vSteps.size());
				bool bResult = true;
				for (auto& step : m_vSteps)
				{
					args.outputLog.clear();
					if (step->isEnabled())
					{
						if (a_dontUseTag)
						{
							if (!step->isTagStep())
								bResult = step->execute(args);
						}
						else
						{
							bResult = step->execute(args);
						}
					}
					else
					{
						bResult = true;
					}
					emit this->sg_update(iIndex, args.outputLog);
					++iIndex;

					if (!bResult)
						break;
				}
				emit this->sg_finished(bResult);
			});
	}
}
