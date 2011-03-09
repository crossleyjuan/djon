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
#include <stdexcept>
#include <iostream>

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
//    QPixmap pixmap(":/img/splash.png");
//    QSplashScreen splash(pixmap);
//    splash.show();
//    app.processEvents();
    checkConfigFile();
    if (errorOcurred()) {
        QMessageBox::critical(NULL, "d-jon", lastErrorDescription(), QMessageBox::Ok, QMessageBox::Cancel);
        qApp->quit();
        return 1;
    }

    MainWindow mainWindow;

    QObject::connect(&app, SIGNAL(messageReceived(const QString&)), &mainWindow, SLOT(restoreWindowState()));

    mainWindow.show();

//    qApp->processEvents();

//    splash.showMessage("Loading...");

//    splash.finish(&mainWindow);
    try {
        return app.exec();
    } catch (std::out_of_range& e) {
        std::cout << "Out of range: " << e.what() << "\n";
    }
}
