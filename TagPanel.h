#pragma once

#include <QWidget>
#include "ui_TagPanel.h"

class TagStep;

class TagPanel : public QWidget
{
	Q_OBJECT

public:
	TagPanel(QWidget *parent = nullptr);
	~TagPanel();
	void loadStep(TagStep* const a_step);

signals:
	void sg_enabled(bool);

private:
	Ui::TagPanelClass ui;
	TagStep* m_pStep;

	void onTag(const QString& a_text);
	void onDir(const QString& a_text);
	void onComment();
	void setEnable(bool a_bEnable);
};
