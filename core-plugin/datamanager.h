#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QtSql>
#include "core-defs.h"
#include "util.h"

class Task;
class Project;

class DataManager
{
public:
    DataManager();
    virtual ~DataManager();
    bool connect();
    QList<CoreProject> projects();
    QList<CoreTask> tasks();

    Map map(Task* task);
    bool saveMap(Map map);

private:
    QSqlDatabase* _dataBase;
    QList<CoreProject> _projects;
    QList<CoreTask> _tasks;

private:
    bool saveDatabaseTimeRecord(Map map);
};

#endif // DATAMANAGER_H
