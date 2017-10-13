#include <QApplication>
#include <list>

#include "src/ui/mainwindow.h"

#include "motorsim.h"
#include "grain.h"
#include "nozzle.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}
