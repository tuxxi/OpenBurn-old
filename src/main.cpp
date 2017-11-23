#include <QApplication>
#include <list>

#include "src/ui/mainwindow.h"

#define QT_AUTO_SCREEN_SCALE_FACTOR 1

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}
