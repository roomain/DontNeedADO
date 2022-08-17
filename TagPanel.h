#pragma once

#include <QWidget>
#include "ui_TagPanel.h"

class TagPanel : public QWidget
{
	Q_OBJECT

public:
	TagPanel(QWidget *parent = nullptr);
	~TagPanel();

private:
	Ui::TagPanelClass ui;
};
