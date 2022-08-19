#include "GitClonePanel.h"
#include "GitStep.h"

GitClonePanel::GitClonePanel(QWidget *parent)
	: QWidget(parent), m_pStep{ nullptr }
{
	ui.setupUi(this);

	QObject::connect(ui.lEdtGit, &QLineEdit::textChanged, this, &GitClonePanel::onUrl);
	QObject::connect(ui.checkBox, &QCheckBox::toggled, this, &GitClonePanel::setEnable);
}

GitClonePanel::~GitClonePanel()
{}

void GitClonePanel::loadStep(GitStep* const a_step)
{
	m_pStep = a_step;
	if (m_pStep)
	{
		ui.checkBox->setChecked(m_pStep->isEnabled());
		ui.lEdtGit->setText(QString::fromLatin1(m_pStep->url()));
	}
}

void GitClonePanel::setEnable(bool a_bEnable)
{
	if (m_pStep)
		m_pStep->setEnable(a_bEnable);
	emit sg_enabled(a_bEnable);
}

void GitClonePanel::onUrl(const QString& a_text)
{
	if (m_pStep)
		m_pStep->setUrl(a_text.toStdString());
}