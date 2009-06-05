#include "logtime.h"

LogTime::LogTime(Task* task) {
    m_task = task;
}

LogTime::LogTime(Job* job) {
    m_job = job;
}

void LogTime::startTimer() {
    m_start = new QDateTime(QDateTime::currentDateTime());
}

void LogTime::stopTimer() {
    m_stop = new QDateTime(QDateTime::currentDateTime());
}

QTime* LogTime::time() {
    int secs = m_stop->secsTo(*m_start);

    int mins = secs / 60;
    secs -= (mins * 60);
    int hours = mins / 60;
    mins -= (hours * 60);

    QTime* time = new QTime(hours, mins, secs);
    return time;
}

void initializeTimers() {
    if (m_LogTimers == NULL) {
        m_LogTimers = new map<string*, LogTime*>();
    }
}

LogTime* createTimer(Task* task) {
    initializeTimers();
    LogTime* log = new LogTime(task);
    string* id = new string("task_" + task->id);
    m_LogTimers->insert(pair<string*, LogTime*>(id, log));
    return log;
}

LogTime* createTimer(Job* job) {
    initializeTimers();
    LogTime* log = new LogTime(job);
    string* id = new string("job_" + job->id);
    m_LogTimers->insert(pair<string*, LogTime*>(id, log));
    return log;
}

LogTime* getTimer(Task* task) {
    initializeTimers();
    map<string*, LogTime*>::iterator iter = m_LogTimers->find(new string("task_" + task->id));
    if (iter != m_LogTimers->end()) {
        return iter->second;
    } else {
        return NULL;
    }
}

LogTime* getTimer(Job* job) {
    initializeTimers();
    map<string*, LogTime*>::iterator iter = m_LogTimers->find(new string("job_" + job->id));
    if (iter != m_LogTimers->end()) {
        return iter->second;
    } else {
        return NULL;
    }
}
