#include "logtime.h"
#include "fileutils.h"
#include "utils.h"
#include "project.h"

map<string, LogTime*>* m_LogTimers;

LogTime::LogTime(Project* project, Task* task) {
    m_project = project;
    m_task = task;
    m_id = uuid();
    m_start = NULL;
    m_stop = NULL;
}

LogTime::LogTime(Project* project, Task* task, QDateTime* initTime, QDateTime* endTime) {
    m_project = project;
    m_task = task;
    m_id = uuid();
    m_start = initTime;
    m_stop = endTime;
}

LogTime::LogTime(Project* project, Job* job) {
    m_project = project;
    m_job = job;
    m_id = uuid();
    m_start = NULL;
    m_stop = NULL;
}

void LogTime::startTimer() {
    m_start = new QDateTime(QDateTime::currentDateTime());
}

void LogTime::stopTimer() {
    m_stop = new QDateTime(QDateTime::currentDateTime());
}

QTime* LogTime::time() {
    QDateTime stop;
    if (m_stop == NULL) {
        stop = QDateTime::currentDateTime();
    } else {
        stop = *m_stop;
    }
    int secs = m_start->secsTo(stop);

    int mins = secs / 60;
    secs -= (mins * 60);
    int hours = mins / 60;
    mins -= (hours * 60);

    QTime* time = new QTime(hours, mins, secs);
    return time;
}

int LogTime::seconds() {
    QTime* t = time();
    int res = (t->hour() * 3600) + (t->minute() * 60) + (t->second());
    return res;
}

LOGTYPE LogTime::type() {
    if (m_task != NULL) {
        return TASK;
    } else {
        return JOB;
    }
}

string* LogTime::id() {
    return m_id;
}

QDateTime* LogTime::initTime() {
    return m_start;
}

QDateTime* LogTime::endTime() {
    return m_stop;
}

Project* LogTime::project() {
    return m_project;
}

Task* LogTime::task() {
    return m_task;
}

Job* LogTime::job() {
    return m_job;
}

/***************************************************************************************/

void initializeTimers() {
    if (m_LogTimers == NULL) {
        m_LogTimers = new map<string, LogTime*>();
    }
}

LogTime* createTimer(Project* project, Task* task) {
    initializeTimers();
    LogTime* log = new LogTime(project, task);
    log->startTimer();
    string id("task_" + task->id);
    m_LogTimers->insert(pair<string, LogTime*>(id, log));
    return log;
}

LogTime* createTimer(Project* project, Job* job) {
    initializeTimers();
    LogTime* log = new LogTime(project, job);
    log->startTimer();
    string id("job_" + job->id);
    m_LogTimers->insert(pair<string, LogTime*>(id, log));
    return log;
}

LogTime* getTimer(Project* project, Task* task) {
    initializeTimers();
    map<string, LogTime*>::iterator iter = m_LogTimers->find(string("task_" + task->id));
    if (iter != m_LogTimers->end()) {
        return iter->second;
    } else {
        return NULL;
    }
}

LogTime* getTimer(Project* project, Job* job) {
    initializeTimers();
    map<string, LogTime*>::iterator iter = m_LogTimers->find("job_" + job->id);
    if (iter != m_LogTimers->end()) {
        return iter->second;
    } else {
        return NULL;
    }
}

void saveTimer(LogTime* log) {
    hashmap* values = new hashmap();
    string logid;

    values->insert(pair<string, string>("log-id", *log->id()));
    values->insert(pair<string, string>("log-start-time", toString(toDouble(*log->initTime()), 6)));
    values->insert(pair<string, string>("log-end-time", toString(toDouble(*log->endTime()), 6)));

    string destPath = log->project()->path + "task" + log->task()->id + "/";
    makedir(destPath.c_str());

    writeFile(destPath + *log->id() + ".log", values);

    if (log->type() == TASK) {
        Task* task = readTask(log->project(), log->task()->id);
        QTime* time = log->time();
        task->totalTime += log->seconds();
        updateTask(log->project(), task);
    }
}

LogTime* readLogTime(Project* project, Task* task, string fileName) {
    string path = project->path;
    string srcPath = project->path + "task" + task->id + "/";
    fileName = srcPath + fileName + ".log";

    hashmap* values = readFile(fileName);

    string id = READ_ELEMENT(values, "log-id");
    QString logStartTime(READ_ELEMENT(values, "log-start-time").c_str());
    double startTime = logStartTime.toDouble();
    QString logEndTime(READ_ELEMENT(values, "log-end-time").c_str());
    double endTime = logEndTime.toDouble();

    LogTime* timer = new LogTime(project, task, toDateTime(startTime), toDateTime(endTime));

    return timer;
}

std::vector<LogTime*> readLogTimes(Project* project, Task* task) {
    string path = project->path;

    vector<string> files;
    std::vector<LogTime*> logs;
    string srcPath = project->path + "task" + task->id + "/";
    int res = getdir(srcPath, files, "log");
    if (res == 0) {
        for (vector<string>::iterator it = files.begin(); it != files.end(); it++) {
            string fileName = (*it);

            LogTime* logtime = readLogTime(project, task, fileName.substr(0, fileName.length() - 4));

            logs.push_back(logtime);
        }
    }
    return logs;
}


