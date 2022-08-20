#pragma once

#include <QWidget>
#include "ui_ConfigurationPanel.h"

class ConfigurationPanel : public QWidget
{
	Q_OBJECT

public:
	ConfigurationPanel(QWidget *parent = nullptr);
	~ConfigurationPanel();

private:
	Ui::ConfigurationPanelClass ui;
};
