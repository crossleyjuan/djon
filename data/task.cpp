/* 
 * File:   Task.cpp
 * Author: cross
 * 
 * Created on July 7, 2010, 11:19 AM
 */

#include "task.h"
#include "data.h"
#include "datautil.h"

#include <stdlib.h>
#include <sstream>
#include <string>

Task::Task(const Task& orig) {
}

Task::Task(Project* project) {
    _project = project;
    _id = NULL;
    _shortDescription = NULL;
    _longDescription = NULL;
    _duration = NULL;
    _startDate = NULL;
    _endDate = NULL;
    _templateName = NULL;
    _status = NULL;
    _totalTime = 0;
    _childCount = 0;

    _logs = new std::vector<TaskLog*>();
    _subTasks = new std::vector<Task*>();
}

Task::~Task() {
}

int Task::childCount() {
    return subTasks()->size();
}

void Task::setTotalTime(DTime* _totalTime) {
    this->_totalTime = _totalTime;
}

DTime* Task::totalTime() {
    vector<Task*>* child = subTasks();
    if (child->size() == 0) {
        long totalTime = 0;
        for (vector<TaskLog*>::iterator iterLog = _logs->begin(); iterLog != _logs->end(); iterLog++) {
            TaskLog* log = *iterLog;
            totalTime += ((*log->end) - (*log->start));
        }
        return new DTime(totalTime);
    } else {
        DTime* tm = new DTime();
        for (vector<Task*>::iterator iter = child->begin(); iter != child->end(); iter++) {
            Task* sub = *iter;
            tm->add(*sub->totalTime());
        }
        return tm;
    }

}

void Task::setStatus(std::string* _status) {
    this->_status = _status;
}

std::string* Task::status() const {
    return _status;
}

void Task::setTemplateName(std::string* _templateName) {
    this->_templateName = _templateName;
}

std::string* Task::templateName() const {
    return _templateName;
}

void Task::setJobs(std::vector<Job*> _jobs) {
    this->_jobs = _jobs;
}

std::vector<Job*> Task::jobs() const {
    return _jobs;
}

void Task::setProject(Project* _project) {
    this->_project = _project;
}

Project* Task::project() const {
    return _project;
}

void Task::setEndDate(DateTime* _endDate) {
    this->_endDate = _endDate;
}

DateTime* Task::endDate() {
    vector<Task*>* child = subTasks();
    if (child->size() == 0) {
        return _endDate;
    } else {
        DateTime* endDate = NULL;
        for (vector<Task*>::iterator iter = child->begin(); iter != child->end(); iter++) {
            Task* sub = *iter;
            if (endDate == NULL) {
                endDate = sub->endDate();
            } else if (*sub->endDate() > *endDate) {
                endDate = sub->endDate();
            }
        }
        return endDate;
    }
}

void Task::setStartDate(DateTime* _startDate) {
    this->_startDate = _startDate;
}

DateTime* Task::startDate() {
    vector<Task*>* child = subTasks();
    if (child->size() == 0) {
        return _startDate;
    } else {
        DateTime* startDate = NULL;
        for (vector<Task*>::iterator iter = child->begin(); iter != child->end(); iter++) {
            Task* sub = *iter;
            if (startDate == NULL) {
                startDate = sub->startDate();
            } else if (*startDate > *sub->startDate()) {
                startDate = sub->startDate();
            }
        }
        return startDate;
    }
}

void Task::setDuration(int _duration) {
    this->_duration = _duration;
}

int Task::duration() const {
    return _duration;
}

void Task::setLongDescription(std::string* _longDescription) {
    this->_longDescription = _longDescription;
}

std::string* Task::longDescription() const {
    return _longDescription;
}

void Task::setShortDescription(std::string* _shortDescription) {
    this->_shortDescription = _shortDescription;
}

std::string* Task::shortDescription() const {
    return _shortDescription;
}

void Task::setId(string* _id) {
    this->_id = _id;
}

string* Task::id() const {
    return _id;
}

std::vector<TaskLog*>* Task::logs() {
    return _logs;
}

Task::Task(Project* project, std::string* taskDef) {
    _project = project;
    _subTasks = NULL;
    _logs = new vector<TaskLog*>();

    hashmap* values = parseTextFormat(*taskDef);

    _id = new std::string(READ_ELEMENT(values, "task-id"));
    _shortDescription =  new std::string(READ_ELEMENT(values, "short-description"));
    _longDescription = new std::string(READ_ELEMENT(values, "long-description"));
    _duration = atoi(READ_ELEMENT(values, "duration").c_str());
    _startDate = new DateTime(READ_ELEMENT(values, "startdate"));
    _endDate = new DateTime(READ_ELEMENT(values, "enddate"));
    _templateName = new string(READ_ELEMENT(values, "template-name"));
    _status = new string(READ_ELEMENT(values, "status"));
    _totalTime = new DTime(atoi(READ_ELEMENT(values, "total-time").c_str()));

    delete(values);
}

char* Task::toChar() {
    std::stringstream ss;

    ss << "task-id:" << *_id << ";\n";
    ss << "short-description:" << *_shortDescription << ";\n";
    ss << "long-description:" << *_longDescription << ";\n";
    ss << "enddate:" << _endDate->toChar() << ";\n";
    ss << "startdate:" << _startDate->toChar() << ";\n";
    ss << "duration:" << _duration << ";\n";
    ss << "status:" << *_status << ";\n";
    ss << "template-name:" << *_templateName << ";\n";
    ss << "total-time:" << _totalTime->secs() << ";\n";

    string ssOut = ss.str();
    char* res = (char*)malloc(ssOut.size() + 1);
    memset(res, 0, ssOut.size() + 1);
    strcpy(res, ssOut.c_str());

    return res;
}

std::vector<Task*>* Task::subTasks() {
    return _project->subTasks(id());
}

void Task::addLog(TaskLog* log) {
    this->_logs->push_back(log);
}

string* Task::nextChildId() {
    vector<Task*>* tasks =subTasks();
    int lastId = 0;
    for (vector<Task*>::iterator iter = tasks->begin(); iter != tasks->end(); iter++) {
        Task* sub = *iter;
        const char* id = sub->id()->c_str();
        id = strrchr(id, '.') + 1;
        int subId = atoi(id);
        if (subId > lastId) {
            lastId = subId;
        }
    }
    stringstream ss;
    lastId++;
    ss << *id() << "." << lastId;

    return new string(ss.str());
}

bool Task::removeLog(TaskLog* log) {
    bool removed = false;
    for (std::vector<TaskLog*>::iterator iter = _logs->begin(); iter != _logs->end(); iter++) {
        TaskLog* taskLog = *iter;
        if (taskLog->id->compare(*log->id) == 0) {
            _logs->erase(iter);
            removed = true;
            break;
        }
    }
    return removed;
}
