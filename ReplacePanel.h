#pragma once

#include <QWidget>
#include "ui_ReplacePanel.h"

class ReplaceStep;

class ReplacePanel : public QWidget
{
	Q_OBJECT

public:
	ReplacePanel(QWidget *parent = nullptr);
	~ReplacePanel();
	void loadStep(ReplaceStep* const a_step);

signals:
	void sg_enabled(bool);

private:
	Ui::ReplacePanelClass ui;
	ReplaceStep* m_pStep;

	void setEnable(bool a_bEnable);
	void onAddToken();
	void addNewLine(const QString& a_token = "", const QString& a_filter = "", const QString& a_value = "");
	void onDeleteRow();
	void onCellChanged(int a_row, int a_col);
	void onEnableVersion(bool a_enable);
	void onVersion(const QString& a_version);
};
