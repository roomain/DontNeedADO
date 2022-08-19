#include "DontNeedADO.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/DontNeedADO/resources/ado.jpg"));
    DontNeedADO w;
    w.show();
    return a.exec();
}
