#ifndef TIMETRACKER_H
#define TIMETRACKER_H

#include <QObject>
//#include <QTimer>
#include "util.h"

#include <QTimer>

class Task;
class TaskLog;
class DTime;

enum TIMERSTATUS {
    RUNNING,
    STOPPED,
    PAUSED
};

class TimeTracker : public QObject
{
Q_OBJECT
public:
    explicit TimeTracker(QObject *parent = 0);
    TIMERSTATUS status();
    Task* task();
    TaskLog* taskLog();
    void moveCurrentRecordToTask(Task* newTask);
    void destroyCurrentRecord();

signals:
    void timeStopped(Task* task, TaskLog* taskLog);
    void timeChanged(DTime& currentLogTime, DTime& taskTotalTime);

public slots:
    void startRecord(Task* task);
    void startRecord(Task* task, TaskLog* taskLog);
    void stopRecord();
    void changeTask(Task* task);
    void changeTaskLog(Task* task, TaskLog* log);

    void timeOut();

private:
    Task* _task;
    TaskLog* _taskLog;
    QTimer* _timer;
    long _secs;
    long _totalTaskTimeSecs;
    long _ticksToSaveLog; // This will record the ticks to save the current running log (avoid Failure)

    TIMERSTATUS _status;
};

#endif // TIMETRACKER_H
