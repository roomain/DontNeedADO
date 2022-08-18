#pragma once

#include <QWidget>
#include "ui_CompilePanel.h"

class CompilePanel : public QWidget
{
	Q_OBJECT

public:
	CompilePanel(QWidget *parent = nullptr);
	~CompilePanel();

private:
	Ui::CompilePanelClass ui;
};
