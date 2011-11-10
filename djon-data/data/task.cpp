// *********************************************************************************************************************
// file:    task.cpp
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

/*
 * File:   Task.cpp
 * Author: cross
 * 
 * Created on July 7, 2010, 11:19 AM
 */

#include "task.h"
#include "data.h"
#include "djonpreferences.h"

#include <stdlib.h>
#include <sstream>
#include <string>

int __currentColor;

int randColor() {
    int color = 0;
    color = (rand() % 205) + 50;
    return color;
}

Task::Task(const Task& orig) {
    _id = orig._id;
    _shortDescription = orig._shortDescription;
    _longDescription = orig._longDescription;
    _duration = orig._duration;
    _startDate = orig._startDate;
    _endDate = orig._endDate;
    _project = orig._project;
    _jobs = orig._jobs;
    _templateName = orig._templateName;
    _status = orig._status;
    _totalTime = orig._totalTime;
    _parentTask = orig._parentTask;

    _logs = orig._logs;
    _taskColor = orig._taskColor;
    _childrenTasks = orig._childrenTasks;
}

Task::Task(Project* project) {
    _project = project;
    _id = NULL;
    _shortDescription = NULL;
    _longDescription = NULL;
    _startDate = NULL;
    _endDate = NULL;
    _templateName = NULL;
    _status = NULL;
    _totalTime = 0;
    _parentTask = NULL;

    _logs = new std::vector<TaskLog*>();
    int red = randColor();
    int green = randColor();
    int blue = randColor();
    _taskColor = QColor(red, green, blue);
}

Task::~Task() {
    delete(_logs);
}

int Task::childCount() const {
    return _childrenTasks.size();
}

DTime Task::totalTime() const {
    DTime result;
    for (vector<TaskLog*>::iterator iterLog = _logs->begin(); iterLog != _logs->end(); iterLog++) {
        TaskLog* log = *iterLog;
        result.add((*log->end) - (*log->start));
    }
    vector<Task*> subtasks = children();
    for (vector<Task*>::iterator iter = subtasks.begin(); iter != subtasks.end(); iter++) {
        Task* sub = *iter;
        result.add(sub->totalTime());
    }
    return result;
}

DTime Task::totalTimeCurrentWeek() const {
    DateTime startDayWeek = DateTime::startDayOfWeek();
    DateTime startDayOfNextWeek = DateTime::startDayOfNextWeek();
    DTime totalTime;
    if (childCount() == 0) {
        for (vector<TaskLog*>::iterator iterLog = _logs->begin(); iterLog != _logs->end(); iterLog++) {
            TaskLog* log = *iterLog;
            if ((*(log->start) >= startDayWeek) &&
                (*(log->end) < startDayOfNextWeek)) {
                totalTime.add((*log->end) - (*log->start));
            }
        }
    } else {
        vector<Task*> subtasks = children();
        for (vector<Task*>::iterator iter = subtasks.begin(); iter != subtasks.end(); iter++) {
            Task* sub = *iter;
            totalTime.add(sub->totalTimeCurrentWeek());
        }
    }
    return totalTime;
}

DTime Task::totalTimeCurrentDay() const {
    DateTime today = DateTime::today();
    DateTime tomorrow = today.addDays(1);
    DTime totalTime;
    if (childCount() == 0) {
        for (vector<TaskLog*>::iterator iterLog = _logs->begin(); iterLog != _logs->end(); iterLog++) {
            TaskLog* log = *iterLog;
            if (*log->start >= today) {
                if (*log->end < tomorrow) {
                    totalTime.add((*log->end) - (*log->start));
                } else {
                    totalTime.add((*log->end) - tomorrow);
                }
            }
        }
    } else {
        vector<Task*> subtasks = children();
        for (vector<Task*>::iterator iter = subtasks.begin(); iter != subtasks.end(); iter++) {
            Task* sub = *iter;
            totalTime.add(sub->totalTimeCurrentDay());
        }
    }
    return totalTime;

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

DateTime* Task::endDate() const {
    vector<Task*> subtasks = children();
    DateTime* result;
    if (childCount() == 0) {
        result = _endDate;
    } else {
        DateTime* endDate = NULL;
        for (vector<Task*>::iterator iter = subtasks.begin(); iter != subtasks.end(); iter++) {
            Task* sub = *iter;
            if (endDate == NULL) {
                endDate = sub->endDate();
            } else if (*sub->endDate() > *endDate) {
                endDate = sub->endDate();
            }
        }
        result = endDate;
    }
    return result;
}

void Task::setStartDate(DateTime* _startDate) {
    this->_startDate = _startDate;
}

DateTime* Task::startDate() const {
    DateTime* result;
    if (childCount() == 0) {
        result = _startDate;
    } else {
        vector<Task*> subtasks = children();
        DateTime* startDate = NULL;
        for (vector<Task*>::iterator iter = subtasks.begin(); iter != subtasks.end(); iter++) {
            Task* sub = *iter;
            if (startDate == NULL) {
                startDate = sub->startDate();
            } else if (*startDate > *sub->startDate()) {
                startDate = sub->startDate();
            }
        }
        result = startDate;
    }
    return result;
}

void Task::setDuration(Duration _duration) {
    this->_duration = _duration;
}

Duration Task::duration() const {
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

std::vector<TaskLog*>* Task::logs(bool includeChildren) const {
    std::vector<TaskLog*>* taskLogs = logs();
    if (includeChildren && (childCount() > 0)) {
        std::vector<Task*> innerTasks = children();
        for (std::vector<Task*>::iterator iter = innerTasks.begin(); iter != innerTasks.end(); iter++) {
            Task* child = *iter;
            std::vector<TaskLog*>* childLogs = child->logs(true);
            taskLogs->insert(taskLogs->end(), childLogs->begin(), childLogs->end());
            delete childLogs;
        }
    }
    return taskLogs;
}

std::vector<TaskLog*>* Task::logs() const {
    std::vector<TaskLog*>* result = new std::vector<TaskLog*>();
    result->insert(result->end(), _logs->begin(), _logs->end());
    return result;
}

Task::Task(Project* project, const std::string taskDef) {
    qDebug("Task::Task(Project* project, std::string* taskDef)");
    _project = project;
    _totalTime = 0;
    _logs = new vector<TaskLog*>();

    hashmap* values = parseTextFormat(taskDef);

    _id = new std::string(READ_ELEMENT(values, "task-id"));
    qDebug("Loading task id: %s", _id->c_str());
    _shortDescription =  new std::string(READ_ELEMENT(values, "short-description"));
    _longDescription = new std::string(READ_ELEMENT(values, "long-description"));
    _duration = Duration(READ_ELEMENT(values, "duration").c_str());
    _startDate = new DateTime(READ_ELEMENT(values, "startdate"));
    _endDate = new DateTime(READ_ELEMENT(values, "enddate"));
    _templateName = new string(READ_ELEMENT(values, "template-name"));
    _status = new string(READ_ELEMENT(values, "status"));

    Template* tpl = readTemplate(*_templateName);
    // Check the current template existance
    if (tpl == NULL) {
        qDebug("Warning: The task with id: %s contains an invalid template: %s", _id->c_str(), _templateName->c_str());
        _templateName = defaultTemplate()->name();
        _status = new string(*defaultTemplate()->statusList().begin());
    }

    int red = randColor();
    int green = randColor();
    int blue = randColor();
    _taskColor = QColor(red, green, blue);
    _parentTask = NULL;
    delete(values);
    qDebug("out Task::Task(Project* project, std::string* taskDef)");
}

char* Task::toChar() {
    std::stringstream ss;

    ss << "task-id:" << *_id << ";\n";
    ss << "short-description:" << *_shortDescription << ";\n";
    if (_longDescription != NULL) {
        ss << "long-description:" << *_longDescription << ";\n";
    }
    ss << "enddate:" << _endDate->toChar() << ";\n";
    ss << "startdate:" << _startDate->toChar() << ";\n";
    ss << "duration:" << _duration.toChar() << ";\n";
    ss << "status:" << *_status << ";\n";
    ss << "template-name:" << *_templateName << ";\n";

    string ssOut = ss.str();
    char* res = (char*)mmalloc(ssOut.size() + 1);
    memset(res, 0, ssOut.size() + 1);
    strcpy(res, ssOut.c_str());

    return res;
}

std::vector<Task*> Task::children() const {
    return _childrenTasks;
}

void Task::addLog(TaskLog* log) {
    this->_logs->push_back(log);
}

string* Task::nextChildId() {
    vector<Task*> tasks = children();
    int lastId = 0;
    for (vector<Task*>::iterator iter = tasks.begin(); iter != tasks.end(); iter++) {
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

bool Task::operator==(const Task& task) const {
    if ((*task.project() == *_project)
        && (task.id()->compare(*_id) == 0)) {
        return true;
    } else {
        return false;
    }
}

void Task::processTemplate() {
    Template* temp = readTemplate(*_templateName);
    std::vector<string> subtasks = temp->subTaskList();
    int subId = 1;
    for (std::vector<string>::iterator iterSub = subtasks.begin(); iterSub != subtasks.end(); iterSub++) {
        string subtask = *iterSub;
        int posPar = subtask.find('(');
        int posFin = subtask.find(')');

        string tempSub = subtask.substr(posPar + 1, posFin - posPar - 1);
        string subTaskName = subtask.substr(0, posPar);

        Task* sub = new Task(_project);
        std::stringstream ssId;
        ssId << *id() << "." << subId++;
        sub->setId(new string(ssId.str()));
        sub->setDuration(Duration(1, 0, 0));
        sub->setEndDate(endDate());
        sub->setShortDescription(new string(subTaskName));
        sub->setStartDate(startDate());
        sub->setStatus(status());
        sub->setTemplateName(new string(tempSub));

        _project->addTask(sub);
        if (errorOcurred()) {
            return;
        }
        sub->processTemplate();
    }
}

DTime TaskLog::totalTime() const {
    return DTime(*end - *start);
}

void Task::setParent(Task* task) {
    _parentTask = task;
}

Task* Task::parent() const {
    return _parentTask;
}

bool Task::isClosed() const {
    Template* tpl = readTemplate(*this->templateName());
    if (tpl != NULL) {
        return (tpl->closedStatus()->compare(*_status) == 0);
    } else {
        return false;
    }
}

void Task::setClosed(bool closed) {
    if (closed) {
        setStatus(readTemplate(*_templateName)->closedStatus());
    } else {
        setStatus(defaultTemplate()->name());
    }
}

QColor Task::taskColor() const {
    return _taskColor;
}

Task* lastTrackedTask(vector<Project*> projects) {
    Element* element = lastTrackedTaskId();
    if (element != NULL) {
        Project* project = searchProject(projects, *element->project());
        if (project != NULL) {
            Task* task = project->task(*element->task());
            return task;
        }
    }
    return NULL;
}

void Task::addChild(Task *task) {
    _childrenTasks.push_back(task);
    task->setParent(this);
}

void Task::removeChild(Task* task) {
    for (vector<Task*>::iterator iterChild = children().begin(); iterChild != children().end(); iterChild++) {
        Task* child = *iterChild;
        if (child == task) {
            children().erase(iterChild);
            break;
        }
    }
    task->setParent(NULL);
}
