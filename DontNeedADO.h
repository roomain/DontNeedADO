#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_DontNeedADO.h"

class DontNeedADO : public QMainWindow
{
    Q_OBJECT

public:
    DontNeedADO(QWidget *parent = nullptr);
    ~DontNeedADO();

private:
    Ui::DontNeedADOClass ui;
};
