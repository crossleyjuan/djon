/*
 * File:   main.cpp
 * Author: cross
 *
 * Created on July 9, 2010, 8:46 AM
 */

#include <QtGui/QApplication>
#include <QtGui>
#include "MainWindow.h"
#include "data.h"
#include "TaskModel.h"

int main(int argc, char *argv[]) {
    // initialize resources, if needed
    // Q_INIT_RESOURCE(resfile);

    QApplication app(argc, argv);
    MainWindow mainWindow;

    mainWindow.show();

    return app.exec();
}
