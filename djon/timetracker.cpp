#include "timetracker.h"
#include "data.h"
#include "util.h"

#include <QDateTime>

TimeTracker::TimeTracker(QObject *parent) :
    QObject(parent)
{
    _status = STOPPED;
    _task = NULL;
    _taskLog = NULL;
    _timer = new QTimer();
    _ticksToSaveLog = 0;
    _lastLapTime = NULL;
    _totalTaskTimeSecs = 0;
    connect(_timer, SIGNAL(timeout()), this, SLOT(timeOut()));
}

void TimeTracker::startRecord(Task* task) {
    if (_status == RUNNING) {
        stopRecord();
    }
    TaskLog* taskLog = createTaskLog(task);
    taskLog->start = new DateTime();
    taskLog->end = new DateTime();
    createTaskLog(task, taskLog);
    startRecord(task, taskLog);
}

void TimeTracker::startRecord(Task* task, TaskLog* taskLog, DateTime* startTime) {
    _task = task;
    _taskLog = taskLog;

    if (startTime == NULL) {
        startTime = new DateTime(QDateTime::currentDateTime());
    }
    _taskLog->start = startTime;
    _status = RUNNING;
    _totalTaskTimeSecs = task->totalTime().totalSecs();
    _ticksToSaveLog = 0;
    _timer->start(1000);
    emit trackerStarted(_task, _taskLog);
}

void TimeTracker::stopRecord() {
    TIMERSTATUS currentStatus = _status;
    _status = STOPPED;
    _timer->stop();
    if (currentStatus == RUNNING) {
        DateTime* stopTime = new DateTime();
        _taskLog->end = stopTime;
        updateTaskLog(_task, _taskLog);
        emit timeStopped(_task, _taskLog);
    }
}


void TimeTracker::changeTask(Task* task) {
    stopRecord();
    startRecord(task);
}

void TimeTracker::changeTaskLog(Task* task, TaskLog* log) {
    stopRecord();
    startRecord(task, log);
}

void TimeTracker::timeOut() {
    DateTime* stopTime = new DateTime();
    _taskLog->end = stopTime;

    _totalTaskTimeSecs++;

    _ticksToSaveLog++;

    // Every minute save the current time, to avoid any failure
    if (_ticksToSaveLog > 60) {
        updateTaskLog(_task, _taskLog);
        _ticksToSaveLog = 0;
    }
    DTime totalTime(_task->totalTime());
    DTime tm = _taskLog->totalTime();
    emit timeChanged(this->_task, tm, totalTime);
}

TIMERSTATUS TimeTracker::status() {
    return _status;
}

Task* TimeTracker::task() {
    return _task;
}

TaskLog* TimeTracker::taskLog() {
    return _taskLog;
}

void TimeTracker::moveCurrentRecordToTask(Task* newTask) {
    _task->removeLog(_taskLog);
    deleteTaskLog(_task, _taskLog);

    _task = newTask;
    _task->addLog(_taskLog);
    createTaskLog(_task, _taskLog);
}

void TimeTracker::moveLappedRecordToTask(Task* newTask) {
    TaskLog* newLog = new TaskLog();
    newLog->id = uuid();
    newLog->start = _taskLog->lastLap;
    newLog->end = _taskLog->end;
    newLog->lastLap = NULL;
    newLog->logDescription = NULL;
    newTask->addLog(newLog);
    createTaskLog(newTask, newLog);

    _taskLog->end = _taskLog->lastLap;
    _taskLog->lastLap = NULL;
    updateTaskLog(_task, _taskLog);

    _taskLog = newLog;
    _task = newTask;
}

void TimeTracker::destroyCurrentRecord() {
    stopRecord();
    _task->removeLog(_taskLog);
    deleteTaskLog(_task, _taskLog);
    _taskLog = NULL;
}

void TimeTracker::removeLapTime() {
    if (_taskLog != NULL) {
        if (_taskLog->lastLap != NULL) {
            _taskLog->end = _taskLog->lastLap;
        }
        _taskLog->lastLap = NULL;
        updateTaskLog(_task, _taskLog);
    }
}

void TimeTracker::startRecordLap() {
    _taskLog->lastLap = new DateTime();
}

void TimeTracker::cleanLapTime() {
    if (_taskLog != NULL) {
        if (_taskLog->lastLap != NULL) {
            delete _taskLog->lastLap;
        }
        _taskLog->lastLap = NULL;
        updateTaskLog(_task, _taskLog);
    }
}

void TimeTracker::dropRecordedTime() {
    TIMERSTATUS currentStatus = _status;

    _status = STOPPED;
    _timer->stop();
    if (currentStatus == RUNNING) {
        emit timeStopped(_task, _taskLog);
    }
}

DTime TimeTracker::trackedTime() {
    return *_taskLog->end - *_taskLog->start;
}
