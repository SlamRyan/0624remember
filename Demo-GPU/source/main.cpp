#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling,true);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps,true);//1019




    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
