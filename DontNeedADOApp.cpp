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

void DontNeedADOApp::setCMakePath(const QString& a_path)
{
	m_appSettings.setValue("cmake_directory", a_path);
}

QString DontNeedADOApp::cmakePath()
{
	return m_appSettings.value("cmake_directory").toString();
}

void DontNeedADOApp::setNugetPath(const QString& a_path)
{
	m_appSettings.setValue("nuget_directory", a_path);
}

QString DontNeedADOApp::nugetPath()
{
	return m_appSettings.value("nuget_directory").toString();
}


void DontNeedADOApp::setMSBuildPath(const QString& a_path)
{
	m_appSettings.setValue("msbuild_directory", a_path);
}

QString DontNeedADOApp::msbuildPath()
{
	return m_appSettings.value("msbuild_directory","C:/Program Files (x86)/Microsoft Visual Studio/2019/Professional/MSBuild/Current/Bin/msbuild.exe").toString();
}

void DontNeedADOApp::setCurrenFile(const QString& a_file)
{
	m_currentFile = a_file;
}

QString DontNeedADOApp::currentFile()const noexcept
{
	return m_currentFile;
}