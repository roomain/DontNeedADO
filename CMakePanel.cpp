#include "CMakePanel.h"
#include "CMakeStep.h"

CMakePanel::CMakePanel(QWidget *parent)
	: QWidget(parent), m_pStep{ nullptr }
{
	ui.setupUi(this);
	QObject::connect(ui.lEdCMakeList, &QLineEdit::textChanged, this, &CMakePanel::onWorkingDir);
	QObject::connect(ui.lEdOutputDir, &QLineEdit::textChanged, this, &CMakePanel::onOutputDir);
	QObject::connect(ui.checkBox, &QCheckBox::toggled, this, &CMakePanel::setEnable);
	QObject::connect(ui.chBxQuote, &QCheckBox::toggled, this, &CMakePanel::onUseQuote);
	QObject::connect(ui.chBxCompilerName, &QCheckBox::toggled, this, &CMakePanel::onUseCompilerName);
	QObject::connect(ui.cboCompiler, &QComboBox::currentIndexChanged, this, &CMakePanel::onChooseCompiler);
	QObject::connect(ui.rBtnWin32, &QRadioButton::toggled, this, &CMakePanel::onCheckConf);
	QObject::connect(ui.rBtnX86, &QRadioButton::toggled, this, &CMakePanel::onCheckConf);
	QObject::connect(ui.rBtnX64, &QRadioButton::toggled, this, &CMakePanel::onCheckConf);

	//QObject::connect(ui.rB, &QCheckBox::toggled, this, &CMakePanel::setEnable);
	ui.cboCompiler->addItem("Visual studio 2019", QVariantList() << "v142" << "Visual Studio 16 2019");
	ui.cboCompiler->addItem("Visual studio 2017", QVariantList() << "v141" << "Visual Studio 15 2017");
	ui.cboCompiler->addItem("Visual studio 2015", QVariantList() << "v140" << "Visual Studio 14 2015");

}

CMakePanel::~CMakePanel()
{}

void CMakePanel::loadStep(CMakeStep* const a_step)
{
	m_pStep = a_step;
	ui.textEdit->clear();
	if (m_pStep)
	{
		ui.lEdOutputDir->setText(QString::fromLatin1(m_pStep->outDir()));
		ui.lEdCMakeList->setText(QString::fromLatin1(m_pStep->workingDir()));
		ui.checkBox->setChecked(m_pStep->isEnabled());
		ui.chBxQuote->setChecked(m_pStep->useQuotes());
		ui.chBxCompilerName->setChecked(m_pStep->useCompilerName());

		auto strPlatform = m_pStep->platform();
		if (strPlatform == "Win32")
		{
			ui.rBtnWin32->setChecked(true);
		}
		else if (strPlatform == "x86")
		{
			ui.rBtnX86->setChecked(true);
		}
		else
		{
			ui.rBtnX64->setChecked(true);
		}
		auto strCompilerVers = m_pStep->compilerVers();
		if (strCompilerVers == "v142")
		{
			ui.cboCompiler->setCurrentIndex(0);
		}
		else if (strCompilerVers == "v141")
		{
			ui.cboCompiler->setCurrentIndex(1);
		}
		else
		{
			ui.cboCompiler->setCurrentIndex(2);
		}
		ui.textEdit->setPlainText(m_pStep->genCmd());
	}
}

void CMakePanel::setEnable(bool a_bEnable)
{
	if (m_pStep)
		m_pStep->setEnable(a_bEnable);
	emit sg_enabled(a_bEnable);
}

void CMakePanel::onChooseCompiler(int a_index)
{
	if (m_pStep)
	{
		ui.checkBox->setChecked(m_pStep->isEnabled());
		auto varList = ui.cboCompiler->itemData(a_index, Qt::UserRole).toList();
		m_pStep->setCompilerVers(varList[0].toString().toStdString());
		m_pStep->setCompilerName(varList[1].toString().toStdString());
		ui.textEdit->clear();
		ui.textEdit->setPlainText(m_pStep->genCmd());
	}
}

void CMakePanel::onCheckConf(bool a_check)
{
	if (a_check && m_pStep)
	{
		if (sender() == ui.rBtnWin32)
		{
			m_pStep->setPlatform("Win32");
			ui.textEdit->clear();
			ui.textEdit->setPlainText(m_pStep->genCmd());
		}
		else if (sender() == ui.rBtnX86)
		{
			m_pStep->setPlatform("x86");
			ui.textEdit->clear();
			ui.textEdit->setPlainText(m_pStep->genCmd());
		}
		else if (sender() == ui.rBtnX64)
		{
			m_pStep->setPlatform("x64");
			ui.textEdit->clear();
			ui.textEdit->setPlainText(m_pStep->genCmd());
		}
	}
}

void CMakePanel::onWorkingDir(const QString& a_path)
{
	if (m_pStep)
	{
		m_pStep->setWorkingDir(a_path.toStdString());
		ui.textEdit->clear();
		ui.textEdit->setPlainText(m_pStep->genCmd());
	}
}

void CMakePanel::onOutputDir(const QString& a_path)
{
	if (m_pStep)
	{
		m_pStep->setOutDir(a_path.toStdString());
		ui.textEdit->clear();
		ui.textEdit->setPlainText(m_pStep->genCmd());
	}
}

void CMakePanel::onUseQuote(const bool a_enable)
{
	if (m_pStep)
	{
		m_pStep->setUseQuotes(a_enable);
		ui.textEdit->clear();
		ui.textEdit->setPlainText(m_pStep->genCmd());
	}
}

void CMakePanel::onUseCompilerName(const bool a_enable)
{
	if (m_pStep)
	{
		m_pStep->setUseCompilerName(a_enable);
		ui.cboCompiler->setEnabled(a_enable);
		ui.label->setEnabled(a_enable);
		ui.textEdit->clear();
		ui.textEdit->setPlainText(m_pStep->genCmd());
	}
}