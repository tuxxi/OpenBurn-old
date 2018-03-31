#include <QApplication>

#include "src/ui/mainwindow.hpp"

#define QT_AUTO_SCREEN_SCALE_FACTOR 1

int main(int argc, char *argv[])
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QApplication app(argc, argv);
    OpenBurnApplication OpenBurn;
    MainWindow w(OpenBurn);
    w.show();
    return app.exec();
}
