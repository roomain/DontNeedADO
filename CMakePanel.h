#pragma once

#include <QWidget>
#include "ui_CMakePanel.h"

class CMakePanel : public QWidget
{
	Q_OBJECT

public:
	CMakePanel(QWidget *parent = nullptr);
	~CMakePanel();

private:
	Ui::CMakePanelClass ui;
};
