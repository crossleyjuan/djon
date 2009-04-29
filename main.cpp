#include <QtGui/QApplication>
#include "mainwindow.h"
#include "fileutils.h"
#include "project.h"

int main(int argc, char *argv[])
{
    hashmap* conf = readFile("djon.conf");

    Project* project;
    hashmap::iterator it = conf->find("last-project-dir");
    if (it != conf->end()) {
        string lastProjectDir = it->second;
        string lastProjectName = conf->find("last-project-name")->second;
        project = readProject(lastProjectDir, lastProjectName);
    }
    QApplication a(argc, argv);
    MainWindow w(project);
    w.show();
    return a.exec();
}
