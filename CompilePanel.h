#pragma once

#include <QWidget>
#include "ui_CompilePanel.h"

class CompileStep;

class CompilePanel : public QWidget
{
	Q_OBJECT

public:
	CompilePanel(QWidget *parent = nullptr);
	~CompilePanel();
	void loadStep(CompileStep* const a_step);

signals:
	void sg_enabled(bool);

private:
	Ui::CompilePanelClass ui;
	CompileStep* m_pStep;

	void setEnable(bool a_bEnable);
	void setRelativePath(const QString& a_value);
	void setConfiguration(const QString& a_value);
	void setPlatform(const QString& a_value);
	void setCompileFlags();
};
