#include <QtGui/QApplication>
#include "mainwindow.h"
#include "fileutils.h"
#include "project.h"

int main(int argc, char *argv[])
{
    char* home = getenv("HOME");
    std::string confFileName = std::string(home) + "/.djon";
    hashmap* conf = readFile(confFileName);

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
