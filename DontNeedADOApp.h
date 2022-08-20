#pragma once
#include <qapplication.h>
#include <qsettings.h>

class DontNeedADOApp : public QApplication
{
	Q_OBJECT

private:
	QSettings m_appSettings;

public:
	DontNeedADOApp(int& argc, char** argv, int = ApplicationFlags);
	~DontNeedADOApp() = default;

	void removeFile(const QString& a_file);
	void addLastFile(const QString& a_file);
	QStringList lastFiles()const noexcept;
};

