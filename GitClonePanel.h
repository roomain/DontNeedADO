#pragma once

#include <QWidget>
#include "ui_GitClonePanel.h"
#include <memory>

class GitStep;

class GitClonePanel : public QWidget
{
	Q_OBJECT

public:
	GitClonePanel(QWidget *parent = nullptr);
	~GitClonePanel();
	void loadStep(GitStep* const a_step);

signals:
	void sg_enabled(bool);

private:
	Ui::GitClonePanelClass ui;
	GitStep* m_pStep;

	void onUrl(const QString& a_text);
	void setEnable(bool a_bEnable);
};
