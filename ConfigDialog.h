#pragma once

#include <QDialog>
#include "ui_ConfigDialog.h"

class ConfigDialog : public QDialog
{
	Q_OBJECT

public:
	ConfigDialog(QWidget *parent = nullptr);
	~ConfigDialog();

private:
	Ui::ConfigDialogClass ui;

	void onBrowseCMake();
	void onBrowseNuget();
	void setCMakePath(const QString& a_value);
	void setNugetPath(const QString& a_value);
};
