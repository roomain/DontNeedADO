#include "ReplacePanel.h"
#include "ReplaceStep.h"

ReplacePanel::ReplacePanel(QWidget *parent)
	: QWidget(parent), m_pStep{ nullptr }
{
	ui.setupUi(this);
	QObject::connect(ui.checkBox, &QCheckBox::toggled, this, &ReplacePanel::setEnable);
	QObject::connect(ui.pBtnAddToken, QOverload<bool>::of(&QPushButton::clicked), this, &ReplacePanel::onAddToken);
	QObject::connect(ui.tbwTokens, &QTableWidget::cellChanged, this, &ReplacePanel::onCellChanged, Qt::UniqueConnection);

	QObject::connect(ui.gbxDLL, &QGroupBox::toggled, this, &ReplacePanel::onEnableVersion);
	QObject::connect(ui.lEdtVersion, &QLineEdit::textChanged, this, &ReplacePanel::onVersion);
}

ReplacePanel::~ReplacePanel()
{}

void ReplacePanel::loadStep(ReplaceStep* const a_step)
{
	ui.tbwTokens->clearContents();
	ui.tbwTokens->setRowCount(0);
	m_pStep = a_step;
	if (m_pStep)
	{
		ui.checkBox->setChecked(m_pStep->isEnabled());
		for(auto token : *m_pStep)
			addNewLine(QString::fromLatin1(token.token), QString::fromLatin1(token.filters), QString::fromLatin1(token.value));
	}
}

void ReplacePanel::setEnable(bool a_bEnable)
{
	if (m_pStep)
		m_pStep->setEnable(a_bEnable);
	emit sg_enabled(a_bEnable);
}

void ReplacePanel::onAddToken()
{
	if (m_pStep)
	{
		m_pStep->addToken("token", "*.*", "val");
		addNewLine("token", "*.*", "val");
	}
}

void ReplacePanel::onEnableVersion(bool a_enable)
{
	if (m_pStep)
		m_pStep->enableVersion(a_enable);
}

void ReplacePanel::onVersion(const QString& a_version)
{
	if (m_pStep)
		m_pStep->setVersion(a_version.toStdString());
}

void ReplacePanel::onCellChanged(int a_row, int a_col)
{
	if (m_pStep)
		m_pStep->replaceToken(a_row, ui.tbwTokens->item(a_row, 0)->text().toStdString(),
			ui.tbwTokens->item(a_row, 1)->text().toStdString(),
			ui.tbwTokens->item(a_row, 2)->text().toStdString());
}

void ReplacePanel::onDeleteRow()
{
	const int iRow = ui.tbwTokens->rowCount();
	for (int i = 0; i < iRow; ++i)
	{
		if (sender() == ui.tbwTokens->cellWidget(i, 3))
		{
			ui.tbwTokens->removeRow(i);
			if (m_pStep)
				m_pStep->removeToken(i);
			break;
		}
	}
}

void ReplacePanel::addNewLine(const QString& a_token, const QString& a_filter, const QString& a_value)
{
	QObject::disconnect(ui.tbwTokens, &QTableWidget::cellChanged, this, &ReplacePanel::onCellChanged);
	int iRow = ui.tbwTokens->rowCount();
	ui.tbwTokens->setRowCount(iRow + 1);
	QTableWidgetItem* pItem = new QTableWidgetItem(a_token);
	ui.tbwTokens->setItem(iRow, 0, pItem);
	pItem = new QTableWidgetItem(a_filter);
	ui.tbwTokens->setItem(iRow, 1, pItem);
	pItem = new QTableWidgetItem(a_value);
	ui.tbwTokens->setItem(iRow, 2, pItem);

	QPushButton* btnDelete = new QPushButton(QIcon(":/DontNeedADO/resources/delete.png"), "");
	QObject::connect(btnDelete, QOverload<bool>::of(&QPushButton::clicked), this, &ReplacePanel::onDeleteRow);
	ui.tbwTokens->setCellWidget(iRow, 3, btnDelete);
	QObject::connect(ui.tbwTokens, &QTableWidget::cellChanged, this, &ReplacePanel::onCellChanged, Qt::UniqueConnection);
}