#include "Pipline.h"
#include "GitStep.h"
#include "CMakeStep.h"
#include "ReplaceStep.h"
#include "CompileStep.h"
#include "NugetStep.h"
#include "TagStep.h"
#include <QXmlStreamWriter>
#include <QDomElement>

int Pipline::stepCount()const
{
	return static_cast<int>(m_vSteps.size());
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
	// TODO
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
		}
	}
	/*if (a_reader.name() == QString("Pipline"))
	{
		if (a_reader.readNextStartElement())
		{
			do
			{
				if (a_reader.name() == QString("GitStep"))
				{
					std::shared_ptr<GitStep> pStep = std::make_shared<GitStep>();
					pStep->load(a_reader);
					m_vSteps.push_back(pStep);
				}
				else if (a_reader.name() == QString("CMakeStep"))
				{
					std::shared_ptr<CMakeStep> pStep = std::make_shared<CMakeStep>();
					pStep->load(a_reader);
					m_vSteps.push_back(pStep);
				}
				else if (a_reader.name() == QString("Pipline"))
				{
					//
				}
				else if (a_reader.name() == QString("ReplaceStep"))
				{
					std::shared_ptr<ReplaceStep> pStep = std::make_shared<ReplaceStep>();
					pStep->load(a_reader);
					m_vSteps.push_back(pStep);
				}
				else if (a_reader.name() == QString("Pipline"))
				{
					//
				}
				qDebug() << a_reader.name();
				//a_reader.readNext();
			} while (a_reader.readNextStartElement());
		}
	}*/
}

bool Pipline::execute(ExecuteArgs& a_retArgs, const bool a_dontUseTag)const
{
	bool bRet = false;
	a_retArgs.workingDirectory = m_workingDir;
	for (auto& step : m_vSteps)
	{
		if (step->isEnabled())
		{
			if (a_dontUseTag)
			{
				if (!step->isTagStep())
					step->execute(a_retArgs);
			}
			else
			{
				step->execute(a_retArgs);
			}
		}
	}
	return bRet;
}
