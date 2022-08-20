#include "NugetPanel.h"
#include "NugetStep.h"

NugetPanel::NugetPanel(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	QObject::connect(ui.checkBox, &QCheckBox::toggled, this, &NugetPanel::setEnable);
	QObject::connect(ui.lEdtDir, &QLineEdit::textChanged, this, &NugetPanel::setRelativePath);
	QObject::connect(ui.lEdFiles, &QLineEdit::textChanged, this, &NugetPanel::setNuspecs);
}

NugetPanel::~NugetPanel()
{}

void NugetPanel::loadStep(NugetStep* const a_step)
{
	m_pStep = a_step;
	if (m_pStep)
	{
		ui.checkBox->setChecked(m_pStep->isEnabled());
		ui.lEdtDir->setText(QString::fromLatin1(m_pStep->relativePath()));
		ui.lEdFiles->setText(QString::fromLatin1(m_pStep->files()));
	}
}

void NugetPanel::setEnable(bool a_bEnable)
{
	if (m_pStep)
		m_pStep->setEnable(a_bEnable);
	emit sg_enabled(a_bEnable);
}

void NugetPanel::setRelativePath(const QString& a_value)
{
	if (m_pStep)
	{
		m_pStep->setRelativePath(a_value.toStdString());
	}
}

void NugetPanel::setNuspecs(const QString& a_value)
{
	if (m_pStep)
	{
		m_pStep->setFiles(a_value.toStdString());
	}
}