#pragma once

#include <QWidget>
#include "ui_ReplacePanel.h"

class ReplacePanel : public QWidget
{
	Q_OBJECT

public:
	ReplacePanel(QWidget *parent = nullptr);
	~ReplacePanel();

private:
	Ui::ReplacePanelClass ui;
};
