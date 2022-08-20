#include "DontNeedADO.h"
#include <QtWidgets/QApplication>
#include "DontNeedADOApp.h"

int main(int argc, char *argv[])
{
    DontNeedADOApp a(argc, argv);
    a.setWindowIcon(QIcon(":/DontNeedADO/resources/ado.jpg"));
    DontNeedADO w;
    w.show();
    return a.exec();
}
