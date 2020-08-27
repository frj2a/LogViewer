// "$Date: $"
// "$Author: $"
// "$Revision: $"

#include "LogViewer.h"

#include <QtGui>
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    LogViewer w;
    w.show();
    return a.exec();
}
