#include "mainwindow.h"

#include <QApplication>
#include <QStyleFactory>
int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    QApplication a(argc, argv);

    QApplication::setStyle(QStyleFactory::create("Fusion"));
    a.setWindowIcon(QIcon(":/icons/img/Ecat.ico"));
    QCoreApplication::setOrganizationName("Fab9_EMS");
    QCoreApplication::setApplicationName("EC_Master_Lite");

    MainWindow w;
    w.show();
    return a.exec();
}
