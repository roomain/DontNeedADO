#include "DontNeedADO.h"
#include <qfiledialog.h>
#include "GitStep.h"
#include "CMakeStep.h"
#include "ReplaceStep.h"
#include "CompileStep.h"
#include "NugetStep.h"
#include "TagStep.h"
#include <qdir.h>
#include <qdiriterator.h>
#include <QDomDocument>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include "DontNeedADOApp.h"

DontNeedADO::DontNeedADO(QWidget *parent)
    : QMainWindow(parent), m_pRecentFiles{ nullptr }
{
    ui.setupUi(this);

    DontNeedADOApp* pApp = static_cast<DontNeedADOApp*>(qApp);
    QStringList lastfiles = pApp->lastFiles();
    if (lastfiles.size() > 0)
    {
        m_pRecentFiles = new QMenu("RecentFiles");
        for (auto file : lastfiles)
        {
            auto pAct = new QAction(file);
            m_pRecentFiles->addAction(pAct);
            QObject::connect(pAct, QOverload<bool>::of(&QAction::triggered), this, &DontNeedADO::onLoadPiplineFromRecent);
        }
        ui.menuPipline->addAction(m_pRecentFiles->menuAction());
    }

    m_pProgress = new QProgressBar;
    ui.statusBar->addPermanentWidget(m_pProgress);

    QObject::connect(ui.pBtnBrowse, QOverload<bool>::of(&QPushButton::clicked), this, &DontNeedADO::onBrowseWorkingDir);
    QObject::connect(ui.pBtnExecutePipline, QOverload<bool>::of(&QPushButton::clicked), this, &DontNeedADO::executePipline);
    QObject::connect(ui.pBtnExecutePiplineWithoutTag, QOverload<bool>::of(&QPushButton::clicked), this, &DontNeedADO::executeWithoutTag);

    QObject::connect(ui.actionAdd_GIT_Clone_Step, QOverload<bool>::of(&QAction::triggered), this, &DontNeedADO::addGitStep);
    QObject::connect(ui.actionAdd_CMake_Step, QOverload<bool>::of(&QAction::triggered), this, &DontNeedADO::addCMakeStep);
    QObject::connect(ui.actionAdd_Replace_String, QOverload<bool>::of(&QAction::triggered), this, &DontNeedADO::addReplaceStep);
    QObject::connect(ui.actionAdd_Compile_Step, QOverload<bool>::of(&QAction::triggered), this, &DontNeedADO::addCompileStep);
    QObject::connect(ui.actionAdd_Nuget_Step, QOverload<bool>::of(&QAction::triggered), this, &DontNeedADO::addNugetStep);
    QObject::connect(ui.actionAdd_Tag_Step, QOverload<bool>::of(&QAction::triggered), this, &DontNeedADO::addTagStep);
    QObject::connect(ui.actionDelete_Step, QOverload<bool>::of(&QAction::triggered), this, &DontNeedADO::onRemoveStep);
    QObject::connect(ui.actionClean, QOverload<bool>::of(&QAction::triggered), this, &DontNeedADO::onClean);

    QObject::connect(ui.actionNew_pipline, QOverload<bool>::of(&QAction::triggered), this, &DontNeedADO::onNewPipline);
    QObject::connect(ui.actionLoad_pipline, QOverload<bool>::of(&QAction::triggered), this, &DontNeedADO::onLoadPipline);
    QObject::connect(ui.actionSave_pipline, QOverload<bool>::of(&QAction::triggered), this, &DontNeedADO::onSavePipline);

    QObject::connect(ui.lstPipline, &QListWidget::itemSelectionChanged, this, &DontNeedADO::itemSelected, Qt::DirectConnection);

    QObject::connect(ui.pageGIT, &GitClonePanel::sg_enabled, this, &DontNeedADO::onEnable);
    QObject::connect(ui.pageCMake, &CMakePanel::sg_enabled, this, &DontNeedADO::onEnable);
    QObject::connect(ui.pageReplace, &ReplacePanel::sg_enabled, this, &DontNeedADO::onEnable);

    ui.stackedWidget->setCurrentIndex(static_cast<int>(STEP_PAGES::LOG_PAGE));

    QObject::connect(&m_ADOPipline, &Pipline::sg_start, this, &DontNeedADO::onPiplineStart);
    QObject::connect(&m_ADOPipline, &Pipline::sg_update, this, &DontNeedADO::onPiplineUpdate);
    QObject::connect(&m_ADOPipline, &Pipline::sg_finished, this, &DontNeedADO::onPiplineFinished);
}

DontNeedADO::~DontNeedADO()
{
    //
}

void DontNeedADO::onBrowseWorkingDir()
{
    QString workingDir = QFileDialog::getExistingDirectory(this, "Working directory");
    if (!workingDir.isEmpty())
    {
        ui.lEdtWorkingDir->setText(workingDir);
        m_ADOPipline.setWorkingDir(workingDir.toStdString());
    }
}


void DontNeedADO::executePipline()
{
    ui.lstPipline->clearSelection();
    if (ui.chBkCleanAtStart->isChecked())
        onClean();

    ui.tEdtLog->clear();
    ui.stackedWidget->setCurrentIndex(static_cast<int>(STEP_PAGES::LOG_PAGE));
    m_ADOPipline.execute(false);    
}

void DontNeedADO::executeWithoutTag()
{
    ui.lstPipline->clearSelection();
    if(ui.chBkCleanAtStart->isChecked())
        onClean();

    ui.tEdtLog->clear();
    ui.stackedWidget->setCurrentIndex(static_cast<int>(STEP_PAGES::LOG_PAGE));
    m_ADOPipline.execute(true);
}

void DontNeedADO::onPiplineStart(int a_StepCount)
{
    ui.menuBar->setEnabled(false);
    ui.mainToolBar->setEnabled(false);
    ui.lstPipline->setEnabled(false);

    m_pProgress->setRange(0, a_StepCount - 1);
    m_pProgress->setValue(0);
}

void DontNeedADO::onPiplineUpdate(int a_step, const QString& a_message)
{
    m_pProgress->setValue(a_step); 
    ui.tEdtLog->append(a_message);
}

void DontNeedADO::onPiplineFinished()
{
    ui.menuBar->setEnabled(true);
    ui.mainToolBar->setEnabled(true);
    ui.lstPipline->setEnabled(true);
}

void DontNeedADO::onClean()
{
    QString pathStr = ui.lEdtWorkingDir->text();
    if (!pathStr.isEmpty())
    {
        QDir path(pathStr);
        if (path.exists())
        {
            path.setFilter(QDir::NoDotAndDotDot | QDir::Files);
            for(QString dirItem : path.entryList())
                path.remove(dirItem);
            path.setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
            for(QString dirItem : path.entryList())
            {
                QDir subDir(path.absoluteFilePath(dirItem));
                subDir.removeRecursively();
            }
        }
    }
}

//-----------------------------------------------
void DontNeedADO::onRemoveStep()
{
    QListWidgetItem* pItem = ui.lstPipline->currentItem();
    if (pItem)
    {
        int iRow = ui.lstPipline->currentRow();
        pItem = ui.lstPipline->takeItem(iRow);
        m_ADOPipline.removeStep(iRow);
        int  iCount = ui.lstPipline->count();
        delete pItem;
    }
}

void DontNeedADO::clear()
{
    ui.lstPipline->clear();
    m_ADOPipline.clear();
}

void DontNeedADO::onNewPipline()
{
    clear();
    ui.stackedWidget->setCurrentIndex(static_cast<int>(STEP_PAGES::LOG_PAGE));
    ui.tEdtLog->clear();
    // TODO
}


void DontNeedADO::onLoadPiplineFromRecent()
{
    QAction* pAct = static_cast<QAction*>(sender());
    if (loadPipline(pAct->text()))
    {
        DontNeedADOApp* pApp = static_cast<DontNeedADOApp*>(qApp);
        pApp->removeFile(pAct->text());
        pApp->addLastFile(pAct->text());
        if (m_pRecentFiles)
        {
            m_pRecentFiles->removeAction(pAct);
            m_pRecentFiles->addAction(pAct);
        }
    }
}

void DontNeedADO::onLoadPipline()
{
    ui.tEdtLog->clear();
    auto filename = QFileDialog::getOpenFileName(this, "Load pipline", "", "Pipline (*.xml)");
    if (loadPipline(filename))
    {
        DontNeedADOApp* pApp = static_cast<DontNeedADOApp*>(qApp);
        pApp->removeFile(filename);
        pApp->addLastFile(filename);
        if (m_pRecentFiles)
        {
            auto lAct = m_pRecentFiles->actions();
            int indexAct = -1;
            for (int i = 0; i < lAct.size() && indexAct < 0; ++i)
            {
                if (lAct[i]->text() == filename)
                    indexAct = i;
            }

            if (indexAct >= 0)
            {
                auto pAct = lAct[indexAct];
                m_pRecentFiles->removeAction(pAct);
                m_pRecentFiles->addAction(pAct);
            }
            else
            {
                auto pAct = new QAction(filename);
                m_pRecentFiles->addAction(pAct);
                QObject::connect(pAct, QOverload<bool>::of(&QAction::triggered), this, &DontNeedADO::onLoadPiplineFromRecent);
            }
        }
        else
        {
            QStringList lastfiles = pApp->lastFiles();
            if (lastfiles.size() > 0)
            {
                m_pRecentFiles = new QMenu("RecentFiles");
                for (auto file : lastfiles)
                {
                    auto pAct = new QAction(file);
                    m_pRecentFiles->addAction(pAct);
                    QObject::connect(pAct, QOverload<bool>::of(&QAction::triggered), this, &DontNeedADO::onLoadPiplineFromRecent);
                }
                ui.menuPipline->addAction(m_pRecentFiles->menuAction());
            }
        }
    }
}

bool DontNeedADO::loadPipline(const QString& a_file)
{
    bool bRet = false;    
    if (!a_file.isEmpty())
    {
        clear();
        ui.stackedWidget->setCurrentIndex(static_cast<int>(STEP_PAGES::LOG_PAGE));
        QFile file(a_file);
        if (file.open(QIODevice::ReadOnly))
        {
            QDomDocument xmlPipline;
            if (xmlPipline.setContent(&file))
            {
                QDomElement root = xmlPipline.firstChildElement();
                if (!root.isNull())
                {
                    ui.lEdtWorkingDir->setText(root.attribute("Working_directory"));
                    ui.chBkCleanAtStart->setChecked(root.attribute("Cleanup_at_start", "0") == "1");
                    m_ADOPipline.setWorkingDir(ui.lEdtWorkingDir->text().toStdString());
                    m_ADOPipline.load(root.firstChildElement("Pipline"));
                }
            }
            file.close();

            // FILL LIST
            for (auto pStep : m_ADOPipline)
            {
                auto pGit = dynamic_cast<GitStep*>(pStep.get());
                if (pGit)
                {
                    QListWidgetItem* pItem = createItem(STEP_PAGES::GIT_PAGE);
                    pItem->setData(Qt::ForegroundRole, pGit->isEnabled() ? QColor(Qt::black) : QColor(Qt::gray));
                }

                auto pCMake = dynamic_cast<CMakeStep*>(pStep.get());
                if (pCMake)
                {
                    QListWidgetItem* pItem = createItem(STEP_PAGES::CMAKE_PAGE);
                    pItem->setData(Qt::ForegroundRole, pCMake->isEnabled() ? QColor(Qt::black) : QColor(Qt::gray));
                }

                auto pReplace = dynamic_cast<ReplaceStep*>(pStep.get());
                if (pReplace)
                {
                    QListWidgetItem* pItem = createItem(STEP_PAGES::REPLACE_PAGE);
                    pItem->setData(Qt::ForegroundRole, pReplace->isEnabled() ? QColor(Qt::black) : QColor(Qt::gray));
                }

                auto pCompile = dynamic_cast<CompileStep*>(pStep.get());
                if (pCompile)
                {
                    QListWidgetItem* pItem = createItem(STEP_PAGES::COMPILE_PAGE);
                    pItem->setData(Qt::ForegroundRole, pCompile->isEnabled() ? QColor(Qt::black) : QColor(Qt::gray));
                }

                auto pTag = dynamic_cast<TagStep*>(pStep.get());
                if (pTag)
                {
                    QListWidgetItem* pItem = createItem(STEP_PAGES::TAG_PAGE);
                    pItem->setData(Qt::ForegroundRole, pTag->isEnabled() ? QColor(Qt::black) : QColor(Qt::gray));
                }

                auto pNuget = dynamic_cast<NugetStep*>(pStep.get());
                if (pNuget)
                {
                    QListWidgetItem* pItem = createItem(STEP_PAGES::NUGET_PAGE);
                    pItem->setData(Qt::ForegroundRole, pNuget->isEnabled() ? QColor(Qt::black) : QColor(Qt::gray));
                }
            }
            bRet = true;
        }
    }
    return bRet;
}

void DontNeedADO::onSavePipline()
{
    auto filename = QFileDialog::getSaveFileName(this, "Save pipline", "", "Pipline (*.xml)");
    if (!filename.isEmpty())
    {
        QFile file(filename);
        if (file.open(QIODevice::WriteOnly))
        {
            QXmlStreamWriter writer(&file);
            writer.setAutoFormatting(true);
            writer.writeStartDocument();

            writer.writeStartElement("DontNeedADO");
            writer.writeAttribute("Working_directory", ui.lEdtWorkingDir->text());
            writer.writeAttribute("Cleanup_at_start", QString("%1").arg(ui.chBkCleanAtStart->isChecked()));

            m_ADOPipline.save(writer);

            writer.writeEndElement();
            writer.writeEndDocument();
            file.close();
        }
    }
}

void DontNeedADO::itemSelected()
{
    QListWidgetItem* pItem = ui.lstPipline->currentItem();
    if (pItem)
    {
        int iRow = ui.lstPipline->currentRow();
        if (iRow < m_ADOPipline.stepCount())
        {
            ui.stackedWidget->setCurrentIndex(pItem->data(Qt::UserRole).toInt());
            switch (static_cast<STEP_PAGES>(pItem->data(Qt::UserRole).toInt()))
            {
            case STEP_PAGES::GIT_PAGE:
                ui.pageGIT->loadStep(dynamic_cast<GitStep*>(m_ADOPipline[iRow].get()));
                break;

            case STEP_PAGES::CMAKE_PAGE:
                ui.pageCMake->loadStep(dynamic_cast<CMakeStep*>(m_ADOPipline[iRow].get()));
                break;

            case STEP_PAGES::COMPILE_PAGE:
                ui.pageCompile->loadStep(dynamic_cast<CompileStep*>(m_ADOPipline[iRow].get()));
                break;

            case STEP_PAGES::NUGET_PAGE:
                ui.pageNuget->loadStep(dynamic_cast<NugetStep*>(m_ADOPipline[iRow].get()));
                break;

            case STEP_PAGES::REPLACE_PAGE:
                ui.pageReplace->loadStep(dynamic_cast<ReplaceStep*>(m_ADOPipline[iRow].get()));
                break;

            case STEP_PAGES::TAG_PAGE:
                ui.pageTag->loadStep(dynamic_cast<TagStep*>(m_ADOPipline[iRow].get()));
                break;

            default:
                break;
            }
        }
    }
}

QListWidgetItem* DontNeedADO::createItem(const STEP_PAGES a_type)
{
    QListWidgetItem* pItem = nullptr;
    switch (a_type)
    {
    case STEP_PAGES::GIT_PAGE:
        pItem = new QListWidgetItem("GIT STEP");
        pItem->setIcon(QIcon(":/DontNeedADO/resources/git.png"));
        pItem->setData(Qt::UserRole, static_cast<int>(STEP_PAGES::GIT_PAGE));
        ui.lstPipline->addItem(pItem);
        break;

    case STEP_PAGES::CMAKE_PAGE:
        pItem = new QListWidgetItem("CMAKE STEP");
        pItem->setIcon(QIcon(":/DontNeedADO/resources/cmake.png"));
        pItem->setData(Qt::UserRole, static_cast<int>(STEP_PAGES::CMAKE_PAGE));
        ui.lstPipline->addItem(pItem);
        break;

    case STEP_PAGES::COMPILE_PAGE:
        pItem = new QListWidgetItem("COMPILE STEP");
        pItem->setIcon(QIcon(":/DontNeedADO/resources/compile.png"));
        pItem->setData(Qt::UserRole, static_cast<int>(STEP_PAGES::COMPILE_PAGE));
        ui.lstPipline->addItem(pItem);
        break;

    case STEP_PAGES::NUGET_PAGE:
        pItem = new QListWidgetItem("NUGET STEP");
        pItem->setIcon(QIcon(":/DontNeedADO/resources/nuget.png"));
        pItem->setData(Qt::UserRole, static_cast<int>(STEP_PAGES::NUGET_PAGE));
        ui.lstPipline->addItem(pItem);
        break;

    case STEP_PAGES::REPLACE_PAGE:
        pItem = new QListWidgetItem("REPLACE STEP");
        pItem->setIcon(QIcon(":/DontNeedADO/resources/replace.png"));
        pItem->setData(Qt::UserRole, static_cast<int>(STEP_PAGES::REPLACE_PAGE));
        ui.lstPipline->addItem(pItem);
        break;

    case STEP_PAGES::TAG_PAGE:
        pItem = new QListWidgetItem("TAG STEP");
        pItem->setIcon(QIcon(":/DontNeedADO/resources/tag.png"));
        pItem->setData(Qt::UserRole, static_cast<int>(STEP_PAGES::TAG_PAGE));
        ui.lstPipline->addItem(pItem);
        break;

    default:
        break;
    }
    return pItem;
}

void DontNeedADO::addGitStep()
{
    QListWidgetItem* pItem = createItem(STEP_PAGES::GIT_PAGE);    
    std::shared_ptr<GitStep> pStep = std::make_shared<GitStep>();
    m_ADOPipline.addStep(pStep);
    ui.lstPipline->setCurrentItem(pItem);
}

void DontNeedADO::addCMakeStep()
{
    QListWidgetItem* pItem = createItem(STEP_PAGES::CMAKE_PAGE);
    std::shared_ptr<CMakeStep> pStep = std::make_shared<CMakeStep>();
    m_ADOPipline.addStep(pStep);
    ui.lstPipline->setCurrentItem(pItem);
}

void DontNeedADO::addReplaceStep()
{
    QListWidgetItem* pItem = createItem(STEP_PAGES::REPLACE_PAGE);
    std::shared_ptr<ReplaceStep> pStep = std::make_shared<ReplaceStep>();
    m_ADOPipline.addStep(pStep);
    ui.lstPipline->setCurrentItem(pItem);
}

void DontNeedADO::addCompileStep()
{
    QListWidgetItem* pItem = createItem(STEP_PAGES::COMPILE_PAGE);
    std::shared_ptr<CompileStep> pStep = std::make_shared<CompileStep>();
    m_ADOPipline.addStep(pStep);
    ui.lstPipline->setCurrentItem(pItem);
}

void DontNeedADO::addNugetStep()
{
    QListWidgetItem* pItem = createItem(STEP_PAGES::NUGET_PAGE);
    std::shared_ptr<NugetStep> pStep = std::make_shared<NugetStep>();
    m_ADOPipline.addStep(pStep);
    ui.lstPipline->setCurrentItem(pItem);
}

void DontNeedADO::addTagStep()
{
    QListWidgetItem* pItem = createItem(STEP_PAGES::TAG_PAGE);
    std::shared_ptr<TagStep> pStep = std::make_shared<TagStep>();
    m_ADOPipline.addStep(pStep);
    ui.lstPipline->setCurrentItem(pItem);
}

void DontNeedADO::onEnable(bool a_bEnable)
{
    auto item = ui.lstPipline->currentItem();
    if (item)
        item->setData(Qt::ForegroundRole, a_bEnable ? QColor(Qt::black) : QColor(Qt::gray));
}