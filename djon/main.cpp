/*
 * File:   main.cpp
 * Author: cross
 *
 * Created on July 9, 2010, 8:46 AM
 */

#include <QtSingleApplication>
#include <QtGui>
#include "MainWindow.h"
#include "data.h"
#include "TaskModel.h"

int main(int argc, char *argv[]) {
    // initialize resources, if needed
    // Q_INIT_RESOURCE(resfile);

    QtSingleApplication app(argc, argv);

    if (app.isRunning()) {
        app.sendMessage(QString("wake"));
        return 0;
    }

    MainWindow mainWindow;

    QObject::connect(&app, SIGNAL(messageReceived(const QString&)), &mainWindow, SLOT(restoreWindowState()));

    mainWindow.show();

    return app.exec();
}
