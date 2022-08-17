#pragma once

#include <QWidget>
#include "ui_NugetPanel.h"

class NugetPanel : public QWidget
{
	Q_OBJECT

public:
	NugetPanel(QWidget *parent = nullptr);
	~NugetPanel();

private:
	Ui::NugetPanelClass ui;
};
