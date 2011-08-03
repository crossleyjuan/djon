#ifndef COREDEFS_H
#define COREDEFS_H

#include <QString>

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
};

struct PluginSettings {
    QString dsn;
    QString userName;
    QString password;
};

#endif // COREDEFS_H
