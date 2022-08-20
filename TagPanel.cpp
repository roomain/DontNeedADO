#include "TagPanel.h"
#include "TagStep.h"

TagPanel::TagPanel(QWidget *parent)
	: QWidget(parent), m_pStep{ nullptr }
{
	ui.setupUi(this);
	QObject::connect(ui.checkBox, &QCheckBox::toggled, this, &TagPanel::setEnable);
	QObject::connect(ui.lEdTag, &QLineEdit::textChanged, this, &TagPanel::onTag);
	QObject::connect(ui.lEdtDir, &QLineEdit::textChanged, this, &TagPanel::onDir);
	QObject::connect(ui.tEdtTagComments, &QPlainTextEdit::textChanged, this, &TagPanel::onComment);
}

TagPanel::~TagPanel()
{}


void TagPanel::loadStep(TagStep* const a_step)
{
	m_pStep = a_step;
	if (m_pStep)
	{
		ui.checkBox->setChecked(m_pStep->isEnabled()); 
		ui.lEdtDir->setText(QString::fromLatin1(m_pStep->relativeDir()));
		ui.lEdTag->setText(QString::fromLatin1(m_pStep->tag()));
		ui.tEdtTagComments->setPlainText(QString::fromLatin1(m_pStep->comments()));
	}
}

void TagPanel::onTag(const QString& a_text)
{
	if (m_pStep)
		m_pStep->setTag(a_text.toStdString());
}

void TagPanel::onDir(const QString& a_text)
{
	if (m_pStep)
		m_pStep->setDir(a_text.toStdString());
}

void TagPanel::onComment()
{
	if (m_pStep)
		m_pStep->setComments(ui.tEdtTagComments->toPlainText().toStdString());
}

void TagPanel::setEnable(bool a_bEnable)
{
	if (m_pStep)
		m_pStep->setEnable(a_bEnable);
	emit sg_enabled(a_bEnable);
}
