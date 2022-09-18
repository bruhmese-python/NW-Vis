#include "qtcontext.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtContext w;
    w.show();
    return a.exec();
}
