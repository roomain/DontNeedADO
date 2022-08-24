#pragma once

#include <QDialog>
#include "ui_VariablesPanel.h"

class VariablesStep;

class VariablesPanel : public QDialog
{
	Q_OBJECT

public:
	VariablesPanel(QWidget *parent = nullptr);
	~VariablesPanel();
	void loadStep(VariablesStep* const a_step);

signals:
	void sg_enabled(bool);

private:
	Ui::VariablesPanelClass ui;
	VariablesStep* m_pStep;

	void setEnable(bool a_bEnable);
	void onAddVariable();
	void addNewLine(const QString& a_name = "", const QString& a_value = "");
	void onDeleteRow();
	void onCellChanged(int a_row, int a_col);
};
