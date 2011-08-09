#ifndef COREDEFS_H
#define COREDEFS_H

#include <QString>
#include "util.h"

class Task;

struct CoreProject {
    QString id;
    QString code;
    QString name;
    int index;
};

struct CoreTask {
    QString id;
    QString code;
    QString name;
    bool isTopLevel;
    int index;
};

struct DjonTask {
    QString projectId;
    QString taskId;
};

struct Map {
    CoreProject coreProject;
    CoreTask coreTask;
    DjonTask task;
    DateTime date;
    DTime time;
    QString description;
};

struct CorePluginSettings {
    QString dsn;
    QString userName;
    QString password;
};

#endif // COREDEFS_H
