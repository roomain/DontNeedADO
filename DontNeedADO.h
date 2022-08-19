#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_DontNeedADO.h"
#include "Pipline.h"

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
        TAG_PAGE
    };

    Ui::DontNeedADOClass ui;
    Pipline m_ADOPipline;

    void clear();
    QListWidgetItem* createItem(const STEP_PAGES a_type);

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
    void onRemoveStep();
    //----------------------
    void itemSelected();
    void onNewPipline();
    void onLoadPipline();
    void onSavePipline();
    //----------------------
    void onClean();
    void onEnable(bool a_bEnable);
};
