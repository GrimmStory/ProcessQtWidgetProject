#include "QtWidgetsApplicationProcess.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtWidgetsApplicationProcess w;
    w.show();
    return a.exec();
}
