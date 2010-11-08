/*
 * File:   main.cpp
 * Author: cross
 *
 * Created on July 9, 2010, 8:46 AM
 */

#include <QtSingleApplication>
#include <QtGui>
#include "MainWindow.h"
#include "config.h"
#include "util.h"

int main(int argc, char *argv[]) {
    // initialize resources, if needed
    // Q_INIT_RESOURCE(resfile);

    QtSingleApplication app(argc, argv);

#ifndef TESTING
    if (app.isRunning()) {
        app.sendMessage(QString("wake"));
        return 0;
    }
#endif
    checkConfigFile();
    if (errorOcurred()) {
        QMessageBox::critical(NULL, "d-jon", lastErrorDescription(), QMessageBox::Ok, QMessageBox::Cancel);
        qApp->quit();
        return 1;
    }

    MainWindow mainWindow;

    QObject::connect(&app, SIGNAL(messageReceived(const QString&)), &mainWindow, SLOT(restoreWindowState()));

    mainWindow.show();

    return app.exec();
}
