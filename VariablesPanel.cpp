#include "VariablesPanel.h"
#include "VariablesStep.h"

VariablesPanel::VariablesPanel(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	QObject::connect(ui.checkBox, &QCheckBox::toggled, this, &VariablesPanel::setEnable);
	QObject::connect(ui.pBtnAddVariable, QOverload<bool>::of(&QPushButton::clicked), this, &VariablesPanel::onAddVariable);
	QObject::connect(ui.tbwVariables, &QTableWidget::cellChanged, this, &VariablesPanel::onCellChanged, Qt::UniqueConnection);
}

VariablesPanel::~VariablesPanel()
{}


void VariablesPanel::loadStep(VariablesStep* const a_step)
{
	ui.tbwVariables->clearContents();
	ui.tbwVariables->setRowCount(0);
	m_pStep = a_step;
	if (m_pStep)
	{
		ui.checkBox->setChecked(m_pStep->isEnabled());
		for (auto var : *m_pStep)
			addNewLine(QString::fromLatin1(var.m_varName), QString::fromLatin1(var.m_varValue));
	}
}

void VariablesPanel::setEnable(bool a_bEnable)
{
	if (m_pStep)
		m_pStep->setEnable(a_bEnable);
	emit sg_enabled(a_bEnable);
}

void VariablesPanel::onAddVariable()
{
	if (m_pStep)
	{
		m_pStep->addVariable("##variable##", "");
		addNewLine("##variable##", "");
	}
}

void VariablesPanel::addNewLine(const QString& a_name, const QString& a_value)
{
	QObject::disconnect(ui.tbwVariables, &QTableWidget::cellChanged, this, &VariablesPanel::onCellChanged);
	int iRow = ui.tbwVariables->rowCount();
	ui.tbwVariables->setRowCount(iRow + 1);
	QTableWidgetItem* pItem = new QTableWidgetItem(a_name);
	ui.tbwVariables->setItem(iRow, 0, pItem);
	pItem = new QTableWidgetItem(a_value);
	ui.tbwVariables->setItem(iRow, 1, pItem);

	QPushButton* btnDelete = new QPushButton(QIcon(":/DontNeedADO/resources/delete.png"), "");
	QObject::connect(btnDelete, QOverload<bool>::of(&QPushButton::clicked), this, &VariablesPanel::onDeleteRow);
	ui.tbwVariables->setCellWidget(iRow, 2, btnDelete);
	QObject::connect(ui.tbwVariables, &QTableWidget::cellChanged, this, &VariablesPanel::onCellChanged, Qt::UniqueConnection);
}

void VariablesPanel::onDeleteRow()
{
	const int iRow = ui.tbwVariables->rowCount();
	for (int i = 0; i < iRow; ++i)
	{
		if (sender() == ui.tbwVariables->cellWidget(i, 3))
		{
			ui.tbwVariables->removeRow(i);
			if (m_pStep)
				m_pStep->deleteVariable(i);
			break;
		}
	}
}

void VariablesPanel::onCellChanged(int a_row, int a_col)
{
	if (m_pStep)
		m_pStep->updateVariable(a_row, ui.tbwVariables->item(a_row, 0)->text().toStdString(),
			ui.tbwVariables->item(a_row, 1)->text().toStdString());
}
