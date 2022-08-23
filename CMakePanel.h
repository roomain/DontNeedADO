#pragma once

#include <QWidget>
#include "ui_CMakePanel.h"
#include <memory>

class CMakeStep;

class CMakePanel : public QWidget
{
	Q_OBJECT

public:
	CMakePanel(QWidget *parent = nullptr);
	~CMakePanel();
	void loadStep(CMakeStep* const a_step);

signals:
	void sg_enabled(bool);

private:
	Ui::CMakePanelClass ui;
	CMakeStep* m_pStep;

	void setEnable(bool a_bEnable);
	void onChooseCompiler(int a_index);
	void onCheckConf(bool a_check);
	void onWorkingDir(const QString& a_path);
	void onOutputDir(const QString& a_path);
	void onUseQuote(const bool a_enable);
	void onUseCompilerName(const bool a_enable);
};
