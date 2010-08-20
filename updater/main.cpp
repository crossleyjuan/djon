#include <QtGui/QApplication>
#include "util.h"
#include "Local.h"
#include <QProcess>
#include <iostream>

int main(int argc, char *argv[])
{
    const char* type = readConfValue("updater");

    const char* exeFileName = "update_djon.exe";
    std::string output = *getTempDir() + "/" + string(exeFileName);
    if (strcmp(type, "local") == 0) {
        retrieveLocalFile(output);
    }
    QProcess* process = new QProcess();
    process->start(QString(output.c_str()));
    cout << "Finished" << endl;
//    process->waitForFinished();
    return 0;
}
