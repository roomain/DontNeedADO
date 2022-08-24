#pragma once
#include <qapplication.h>
#include <qsettings.h>

class DontNeedADOApp : public QApplication
{
	Q_OBJECT

private:
	QSettings m_appSettings;
	QString m_currentFile;

public:
	DontNeedADOApp(int& argc, char** argv, int = ApplicationFlags);
	~DontNeedADOApp() = default;

	void removeFile(const QString& a_file);
	void addLastFile(const QString& a_file);
	QStringList lastFiles()const noexcept;

	void setCMakePath(const QString& a_path);
	QString cmakePath();
	void setNugetPath(const QString& a_path);
	QString nugetPath();

	void setCurrenFile(const QString& a_file);
	QString currentFile()const noexcept;
};

