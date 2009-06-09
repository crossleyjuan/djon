#ifndef LOGTIME_H
#define LOGTIME_H

#include "project.h"
#include <QDateTime>
#include <QTime>
#include "utils.h"
#include <string>

enum LOGTYPE {
    TASK,
    JOB
};

class LogTime
{
public:
    LogTime(Project* project, Task* task);
    LogTime(Project* project, Job* job);
    void startTimer();
    void stopTimer();
    QTime* time();
    int seconds();
    string* id();
    QDateTime* initTime();
    QDateTime* endTime();
    LOGTYPE type();
    Project* project();
    Task* task();
    Job* job();

private:
    Project* m_project;
    Task* m_task;
    Job* m_job;
    QDateTime* m_start;
    QDateTime* m_stop;
    string* m_id;
};


LogTime* createTimer(Project* project, Task* task);
LogTime* createTimer(Project* project, Job* job);

LogTime* getTimer(Project* project, Task* task);
LogTime* getTimer(Project* project, Job* job);

void saveTimer(LogTime* log);

#endif // LOGTIME_H
