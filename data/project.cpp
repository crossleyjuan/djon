/* 
 * File:   project.cpp
 * Author: cross
 * 
 * Created on July 7, 2010, 11:13 AM
 */

#include "project.h"
#include "datautil.h"
#include "util.h"
#include "data.h"
#include <string.h>
#include <stdlib.h>
#include <sstream>

Project::Project() {
    _tasks = new vector<Task*>();
    _tasksMap = new std::map<string, Task*>();
    _name = NULL;
    _description = NULL;
}

Project::Project(string projDef) {
    hashmap* values = parseTextFormat((char*)projDef.c_str());

    _name = new string(READ_ELEMENT(values, "project-name"));
    _description = new string(READ_ELEMENT(values, "project-description"));

    _tasks = new vector<Task*>();
    _tasksMap = new std::map<string, Task*>();
    delete(values);
}

Project::Project(const Project& orig) {
    this->_description = orig._description;
    this->_name = orig._name;
    this->_tasks = orig._tasks;
    this->_tasksMap = orig._tasksMap;
}

Project::~Project() {
    delete(_name);
    delete(_description);
    delete(_tasks);
    delete(_tasksMap);
}

string* Project::name() {
    return _name;
}

void Project::setName(string* name) {
    _name = name;
}

void Project::setDescription(string* description) {
    _description = description;
}

string* Project::description() {
    return _description;
}

std::vector<Task*>* Project::tasks() {
    return _tasks;
}

void Project::addTask(Task* task) {
    _tasks->push_back(task);
    _tasksMap->insert(pair<string, Task*>(*task->id(), task));
}

Task* Project::task(string& id) {
    return (*_tasksMap)[id];
}

// prefix should be 1, 2.1, etc.
std::vector<Task*>* Project::subTasks(string* prefix) {
    std::vector<Task*>* res = new std::vector<Task*>();

    for (std::vector<Task*>::iterator iterTask = _tasks->begin(); iterTask != _tasks->end(); iterTask++) {
        Task* task = *iterTask;
        string* id = task->id();
        if (prefix->size() == 0) {
            if (id->find(".") == -1) {
                res->push_back(task);
            }
            // starts with but it's not equal
            // if prefix is "1" and the id is "1.1" it will return true,
            // if the id is "1" then it'll return false
        } else if ((id->compare(0, prefix->length(), *prefix) == 0) &&
            (id->compare(*prefix) != 0)) {
            res->push_back(task);
        }
    }
    return res;
}

std::vector<Task*>* Project::subTasks() {
    return subTasks(new std::string(""));
}

DateTime* Project::startDate() {
    vector<Task*>* child = tasks();
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

DateTime* Project::endDate() {
    vector<Task*>* child = tasks();
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

DTime* Project::totalTime() {
    vector<Task*>* tasks = _tasks;
    DTime* result = new DTime();
    for (vector<Task*>::iterator iter = tasks->begin(); iter != tasks->end(); iter++) {
        Task* task = *iter;
        if (task->childCount() == 0) {
            result->add(*task->totalTime());
        }
    }

    return result;
}

char* Project::toChar() {
    std::stringstream ss;
    ss << "project-name:" << *_name  << ";\n";
    ss << "project-description:" << *_description << ";\n";

    string ssOut =  ss.str();
    char* res = (char*)malloc(ssOut.size() + 1);
    memset(res, 0, ss.str().size() + 1);
    strcpy(res, ssOut.c_str());

    return res;
}

std::string* Project::nextChildId() {
    vector<Task*>* tasks = subTasks();
    int lastId = 0;
    for (vector<Task*>::iterator iter = tasks->begin(); iter != tasks->end(); iter++) {
        Task* sub = *iter;
        const char* id = sub->id()->c_str();
        int subId = atoi(id);
        if (subId > lastId) {
            lastId = subId;
        }
    }
    stringstream ss;

    lastId++;
    ss << lastId;
    std::string* res = new std::string(ss.str());

    return res;
}
