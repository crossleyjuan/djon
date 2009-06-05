#ifndef LOGTIME_H
#define LOGTIME_H

#include "project.h"
#include <QDateTime>
#include <QTime>
#include "utils.h"
#include <string>

class LogTime
{
public:
    LogTime(Task* task);
    LogTime(Job* job);
    void startTimer();
    void stopTimer();
    QTime* time();

private:
    Task* m_task;
    Job* m_job;
    QDateTime* m_start;
    QDateTime* m_stop;
};


map<string*, LogTime*>* m_LogTimers;

LogTime* createTimer(Task* task);
LogTime* createTimer(Job* job);

LogTime* getTimer(Task* task);
LogTime* getTimer(Job* job);

#endif // LOGTIME_H
