#include "m2modder2.h"

#include <QtGui>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    M2Modder2 w;
    w.show();
    return a.exec();
}
