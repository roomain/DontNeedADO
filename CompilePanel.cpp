#include "CompilePanel.h"
#include "CompileStep.h"

CompilePanel::CompilePanel(QWidget *parent)
	: QWidget(parent), m_pStep{ nullptr }
{
	ui.setupUi(this);
	QObject::connect(ui.checkBox, &QCheckBox::toggled, this, &CompilePanel::setEnable);
	QObject::connect(ui.lEdtSLNPath, &QLineEdit::textChanged, this, &CompilePanel::setRelativePath);
	QObject::connect(ui.lEdtConf, &QLineEdit::textChanged, this, &CompilePanel::setConfiguration);
	QObject::connect(ui.lEdtPlatform, &QLineEdit::textChanged, this, &CompilePanel::setPlatform);
	QObject::connect(ui.ptEdtOptions, &QPlainTextEdit::textChanged, this, &CompilePanel::setCompileFlags);
}

CompilePanel::~CompilePanel()
{}

void CompilePanel::setEnable(bool a_bEnable)
{
	if (m_pStep)
		m_pStep->setEnable(a_bEnable);
	emit sg_enabled(a_bEnable);
}

void CompilePanel::loadStep(CompileStep* const a_step)
{
	m_pStep = a_step;
	if (m_pStep)
	{
		ui.checkBox->setChecked(m_pStep->isEnabled());
		ui.lEdtSLNPath->setText(QString::fromLatin1(m_pStep->relativePath()));
		ui.lEdtConf->setText(QString::fromLatin1(m_pStep->configuration()));
		ui.lEdtPlatform->setText(QString::fromLatin1(m_pStep->platform()));
		ui.ptEdtOptions->setPlainText(QString::fromLatin1(m_pStep->conpileFlags()));
	}
}


void CompilePanel::setRelativePath(const QString& a_value)
{
	if (m_pStep)
	{
		m_pStep->setRelativePath(a_value.toStdString());
	}
}

void CompilePanel::setConfiguration(const QString& a_value)
{
	if (m_pStep)
	{
		m_pStep->setConfiguration(a_value.toStdString());
	}
}

void CompilePanel::setPlatform(const QString& a_value)
{
	if (m_pStep)
	{
		m_pStep->setPlatform(a_value.toStdString());
	}
}

void CompilePanel::setCompileFlags()
{
	if (m_pStep)
	{
		m_pStep->setCompileFlags(ui.ptEdtOptions->toPlainText().toStdString());
	}
}