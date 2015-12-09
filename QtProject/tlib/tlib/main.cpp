/*******************************************************************
 *  Copyright(c) 2015-2017 Nanjing telesing
 *  All rights reserved.
 *
 *  文件名称: main
 *  简要描述: main 方法
 *
 *  当前版本:1.1
 *  作   者:Lee.liang
 *  日   期:
 *  说   明:
 *
 *  取代版本:1.0
 *  作   者:
 *  日   期:
 *  说   明:
 ******************************************************************/
#include "mainwindow.h"
#include <QApplication>

#include "./tlib/interface/i4cpp/tlib.h"
#include "./tlib/core/core.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
