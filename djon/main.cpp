// *********************************************************************************************************************
// file:    main.cpp
// author:  Juan Pablo Crossley (crossleyjuan@gmail.com)
// created: Oct/1/2010
// updated: Nov/7/2011
// license: GPL 2
//
// This file is part of the djon project, all the source code is licensed under the terms of the GPL license,
// the application and libraries are provided as-is and free of use under the terms explained in the file COPYING.GPL
// the authors are not responsible for any problem that this program, or any of its libraries, may cause.
// Its authors create this application in order to make the world a better place to live, but you should use it on
// your own risks.
// 
// Also, be adviced that, the GPL license force the committers to ensure this application will be free of use, thus
// if you do any modification you will be required to provide it for free unless you use it for personal use (you may 
// charge yourself if you want), bare in mind that you will be required to provide a copy of the license terms that ensures
// this program will be open sourced and all its derivated work will be too.
// *********************************************************************************************************************

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
