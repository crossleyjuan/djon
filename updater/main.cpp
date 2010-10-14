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
