// *********************************************************************************************************************
// file:
// author: Juan Pablo Crossley (crossleyjuan@gmail.com)
// created:
// updated:
// license:
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

#include <QtGui/QApplication>
#include "util.h"
#include "Local.h"
#include <QProcess>
#include <iostream>

int main(int argc, char *argv[])
{
    const char* exeFileName = "update_djon.exe";
    std::string output = *getTempDir() + "/" + string(exeFileName);
    QProcess* process = new QProcess();
    process->start(QString(output.c_str()));
    return 0;
}
