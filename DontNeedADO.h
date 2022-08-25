#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_DontNeedADO.h"
#include "Pipline.h"
#include <qprogressbar.h>

class QListWidgetItem;

class DontNeedADO : public QMainWindow
{
    Q_OBJECT

public:
    DontNeedADO(QWidget *parent = nullptr);
    ~DontNeedADO();

private:
    enum class STEP_PAGES : int
    {
        LOG_PAGE = 0,
        GIT_PAGE,
        CMAKE_PAGE,
        REPLACE_PAGE,
        COMPILE_PAGE,
        NUGET_PAGE,
        TAG_PAGE,
        VAR_PAGE
    };

    Ui::DontNeedADOClass ui;
    QProgressBar* m_pProgress;
    Pipline m_ADOPipline;
    QMenu* m_pRecentFiles;

    void clear();
    QListWidgetItem* createItem(const STEP_PAGES a_type);
    void onRowsMoved(const QModelIndex& parent, int start, int end, const QModelIndex& destination, int row);

    void onBrowseWorkingDir();
    void executePipline();
    void executeWithoutTag();
    //-----------------------
    void addGitStep();
    void addCMakeStep();
    void addReplaceStep();
    void addCompileStep();
    void addNugetStep();
    void addTagStep();
    void addVarStep();
    void onLogs();
    void onRemoveStep();
    //----------------------
    void itemSelected();
    void onNewPipline();
    void onLoadPipline();
    void onLoadPiplineFromRecent();
    bool loadPipline(const QString& a_file);
    void onSavePipline();
    void onSavePiplineAs();
    void saveFile(const QString& a_file);
    //----------------------
    void onClean();
    void onEnable(bool a_bEnable);

    void onPiplineStart(int a_StepCount);
    void onPiplineUpdate(int a_step, const QString& a_message);
    void onPiplineFinished(bool a_result);

    void createRecentMenu();
    void resetRecentMenu(const QString& a_newFile);
    void onConfiguration();
};
