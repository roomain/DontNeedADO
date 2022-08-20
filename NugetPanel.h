#pragma once

#include <QWidget>
#include "ui_NugetPanel.h"

class NugetStep;

class NugetPanel : public QWidget
{
	Q_OBJECT

public:
	NugetPanel(QWidget *parent = nullptr);
	~NugetPanel();
	void loadStep(NugetStep* const a_step);

signals:
	void sg_enabled(bool);

private:
	Ui::NugetPanelClass ui;
	NugetStep* m_pStep;

	void setEnable(bool a_bEnable);
	void setRelativePath(const QString& a_value);
	void setNuspecs(const QString& a_value);
};
