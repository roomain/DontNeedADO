#include "ConfigDialog.h"
#include "DontNeedADOApp.h"
#include <qfiledialog.h>

ConfigDialog::ConfigDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);


	DontNeedADOApp* pApp = static_cast<DontNeedADOApp*>(qApp);
	ui.lEdtCMake->setText(pApp->cmakePath());
	ui.lEdtNuget->setText(pApp->nugetPath());


	QObject::connect(ui.pBtnBrowseCMake, QOverload<bool>::of(&QPushButton::clicked), this, &ConfigDialog::onBrowseCMake);
	QObject::connect(ui.pBtnBrowseNuget, QOverload<bool>::of(&QPushButton::clicked), this, &ConfigDialog::onBrowseNuget);

	QObject::connect(ui.lEdtCMake, &QLineEdit::textChanged, this, &ConfigDialog::setCMakePath);
	QObject::connect(ui.lEdtNuget, &QLineEdit::textChanged, this, &ConfigDialog::setNugetPath);
}

ConfigDialog::~ConfigDialog()
{}

void ConfigDialog::setCMakePath(const QString& a_value)
{
	DontNeedADOApp* pApp = static_cast<DontNeedADOApp*>(qApp);
	pApp->setCMakePath(a_value);
}

void ConfigDialog::setNugetPath(const QString& a_value)
{
	DontNeedADOApp* pApp = static_cast<DontNeedADOApp*>(qApp);
	pApp->setNugetPath(a_value);
}

void ConfigDialog::onBrowseCMake()
{
	QString path = QFileDialog::getExistingDirectory(this, "Get CMake directory");
	if (!path.isEmpty())
	{
		ui.lEdtCMake->setText(path);
		//DontNeedADOApp* pApp = static_cast<DontNeedADOApp*>(qApp);
		//pApp->setCMakePath(path);
	}
}

void ConfigDialog::onBrowseNuget()
{
	QString path = QFileDialog::getExistingDirectory(this, "Get Nuget directory");
	if (!path.isEmpty())
	{
		ui.lEdtNuget->setText(path);
		//DontNeedADOApp* pApp = static_cast<DontNeedADOApp*>(qApp);
		//pApp->setNugetPath(path);
	}
}