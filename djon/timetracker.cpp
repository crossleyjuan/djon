// *********************************************************************************************************************
// file:    timetracker.cpp
// author:  Juan Pablo Crossley (crossleyjuan@gmail.com)
// created: Oct/1/2010
// updated: Nov/7/2011
// license: GPL 2
//
// This file is part of the djon project, all the source code is licensed under the terms of the GPL license,
// the application and libraries are provided as-is and free of use under the terms explained in the file COPYING.GPL
// the authors are not responsible for any problem that this program, or any of its libraries, may cause.
// Its authors create this application in order to make the world a better place to live, but you should use it on
// your own risks.
// 
// Also, be adviced that, the GPL license force the committers to ensure this application will be free of use, thus
// if you do any modification you will be required to provide it for free unless you use it for personal use (you may 
// charge yourself if you want), bare in mind that you will be required to provide a copy of the license terms that ensures
// this program will be open sourced and all its derivated work will be too.
// *********************************************************************************************************************

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
    saveProject(task->project());
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
    _taskLog->activeLog = true;
    emit trackerStarted(_task, _taskLog);
}

void TimeTracker::stopRecord() {
    TIMERSTATUS currentStatus = _status;
    _status = STOPPED;
    _timer->stop();
    if (currentStatus == RUNNING) {
        DateTime* stopTime = new DateTime();
        _taskLog->end = stopTime;
        _taskLog->activeLog = false;
        saveProject(_task->project());
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
        saveProject(_task->project());
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

    _task = newTask;
    _task->addLog(_taskLog);
    saveProject(_task->project());
}

void TimeTracker::moveLappedRecordToTask(Task* newTask) {
    TaskLog* newLog = new TaskLog();
    newLog->id = uuid();
    newLog->start = _taskLog->lastLap;
    newLog->end = _taskLog->end;
    newLog->lastLap = NULL;
    newLog->logDescription = NULL;
    newTask->addLog(newLog);

    _taskLog->end = _taskLog->lastLap;
    _taskLog->lastLap = NULL;

    _taskLog = newLog;
    _task = newTask;

    saveProject(newTask->project());
}

void TimeTracker::destroyCurrentRecord() {
    stopRecord();
    _task->removeLog(_taskLog);
    _taskLog = NULL;
    saveProject(_task->project());
}

void TimeTracker::removeLapTime() {
    if (_taskLog != NULL) {
        if (_taskLog->lastLap != NULL) {
            _taskLog->end = _taskLog->lastLap;
        }
        _taskLog->lastLap = NULL;
        saveProject(_task->project());
    }
}

void TimeTracker::startRecordLap(DateTime* since) {
    if (since == NULL) {
        _taskLog->lastLap = new DateTime();
    } else {
        _taskLog->lastLap = new DateTime(*since);
    }
    _lastLapTime = _taskLog->lastLap;
}

void TimeTracker::cleanLapTime() {
    if (_taskLog != NULL) {
        if (_taskLog->lastLap != NULL) {
            delete _taskLog->lastLap;
        }
        _taskLog->lastLap = NULL;
        saveProject(_task->project());
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

DateTime* TimeTracker::lastLapTime() const {
    return _lastLapTime;
}
