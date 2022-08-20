#include "DontNeedADOApp.h"

#define MAX_FILES 10

DontNeedADOApp::DontNeedADOApp(int& argc, char** argv, int) : QApplication(argc, argv),
	m_appSettings{"dontNeedADO.ini", QSettings::IniFormat }
{
	//
}

void DontNeedADOApp::removeFile(const QString& a_file)
{
	QStringList files = lastFiles();
	files.removeAll(a_file);
	QString list;
	for (int i = 0; i < files.size() - 1; ++i)
		list += files[i] + ";";

	if(!files.isEmpty())
		list += files.last();
	m_appSettings.setValue("last_files", list);
}

void DontNeedADOApp::addLastFile(const QString& a_file)
{
	QStringList files = lastFiles();
	if (files.size() >= 10)
	{
		while (files.size() >= 10)
			files.pop_front();
	}
	files.push_back(a_file);
	QString list;
	for (int i = 0; i < files.size() - 1; ++i)
		list += files[i] + ";";
	
	if (!files.isEmpty())
		list += files.last();
	m_appSettings.setValue("last_files", list);
}

QStringList DontNeedADOApp::lastFiles()const noexcept
{
	QString lstFiles = m_appSettings.value("last_files").toString();
	auto lst = lstFiles.split(';');
	lst.removeAll("");
	return lst;
}