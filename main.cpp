/********************
*AGV自动导航软件
* Author:Bo.Z
* Date:20160304
*********************/


#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
