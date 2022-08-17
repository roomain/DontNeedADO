#include "DontNeedADO.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DontNeedADO w;
    w.show();
    return a.exec();
}
