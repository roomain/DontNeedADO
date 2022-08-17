#pragma once

#include <QWidget>
#include "ui_GitClonePanel.h"

class GitClonePanel : public QWidget
{
	Q_OBJECT

public:
	GitClonePanel(QWidget *parent = nullptr);
	~GitClonePanel();

private:
	Ui::GitClonePanelClass ui;
};
