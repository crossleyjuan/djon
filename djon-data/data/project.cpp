/* 
 * File:   project.cpp
 * Author: cross
 * 
 * Created on July 7, 2010, 11:13 AM
 */

#include "project.h"
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
    _projectFileName = NULL;
    _id = NULL;
    _projectDefaultCalendar = defaultCalendar();
}

Project::Project(string projDef) {
    qDebug("Project::Project(string projDef)");
    Project();
    hashmap* values = parseTextFormat((char*)projDef.c_str());

    _id = new string(READ_ELEMENT(values, "project-id"));
    if (_id->length() == 0) {
        _id = uuid();
    }
    _name = new string(READ_ELEMENT(values, "project-name"));
    _description = new string(READ_ELEMENT(values, "project-description"));
    std::string calendarName = std::string(READ_ELEMENT(values, "project-calendar"));
    if (calendarName.length() == 0) {
        _projectDefaultCalendar = defaultCalendar();
    } else {
        _projectDefaultCalendar = calendar(calendarName);
    }
    _tasks = new vector<Task*>();
    _tasksMap = new std::map<string, Task*>();
    delete(values);
    qDebug("out Project::Project(string projDef)");
}

Project::Project(const Project& orig) {
    this->_id = orig._id;
    this->_description = orig._description;
    this->_name = orig._name;
    this->_tasks = orig._tasks;
    this->_tasksMap = orig._tasksMap;
    this->_projectFileName = orig._projectFileName;
    this->_projectDefaultCalendar = orig._projectDefaultCalendar;
}

Project::~Project() {
    delete(_id);
    delete(_name);
    delete(_description);
    delete(_tasks);
    delete(_tasksMap);
    delete(_projectFileName);
}

string* Project::id() const {
    return _id;
}

void Project::setId(string* id) {
    _id = id;
}

string* Project::name() const {
    return _name;
}

void Project::setName(string* name) {
    _name = name;
}

void Project::setDescription(string* description) {
    _description = description;
}

string* Project::description() const {
    return _description;
}

std::vector<Task*>* Project::tasks() const {
    return _tasks;
}

void Project::addTask(Task* task) {
    Task* checkTask = this->task(*task->id());
    if (checkTask != NULL) {
        setLastError(4, "The task %s alread exists in this project, the file: %s.tsk contains an invalid data. Please fix this manually", task->id()->c_str(), projectFileName()->c_str());
        return;
    }
    task->setProject(this);
    _tasks->push_back(task);
    _tasksMap->insert(pair<string, Task*>(*task->id(), task));

    // Finds the parent task to be added to
    Task* parentTask = NULL;
    string id = *task->id();
    if (countChar(id.c_str(), '.') > 0) {
        id = id.substr(0, id.rfind('.'));
        parentTask = this->task(id);
    } else {
        parentTask = NULL;
    }
    if (parentTask != NULL) {
        parentTask->addChild(task);
    }
}

Task* Project::task(string id) {
    map<string, Task*>::iterator iter = _tasksMap->find(id);
    if (iter != _tasksMap->end()) {
        return iter->second;
    } else {
        return NULL;
    }
}

// prefix should be 1, 2.1, etc.
std::vector<Task*>* Project::subTasks(string* prefix) const {
    std::vector<Task*>* res = new std::vector<Task*>();

    for (std::vector<Task*>::iterator iterTask = _tasks->begin(); iterTask != _tasks->end(); iterTask++) {
        Task* task = *iterTask;
        string* id = task->id();
        int dotsInPrefix = countChar(prefix->c_str(), '.');
        int dotsInId = countChar(id->c_str(), '.');
        if ((prefix->length() == 0) &&
            (dotsInId == 0)){
            res->push_back(task);
        } else if ((dotsInId -1) == dotsInPrefix) {
            int posNextDot = id->find_first_of('.', prefix->length());
            string currentPrefix = id->substr(0, posNextDot);
            if (currentPrefix.compare(*prefix) == 0) {
                res->push_back(task);
            }
        }
    }
    return res;
}

std::vector<Task*>* Project::subTasks() const {
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

DTime Project::totalTime() const {
    vector<Task*>* tasks = _tasks;
    DTime result;
    for (vector<Task*>::iterator iter = tasks->begin(); iter != tasks->end(); iter++) {
        Task* task = *iter;
        if (task->childCount() == 0) {
            result = result + task->totalTime();
        }
    }

    return result;
}

DTime Project::totalTimeCurrentWeek() const {
    vector<Task*>* tasks = _tasks;

    DTime result;
    for (vector<Task*>::iterator iter = tasks->begin(); iter != tasks->end(); iter++) {
        Task* task = *iter;
        if (task->childCount() == 0) {
            result.add(task->totalTimeCurrentWeek());
        }
    }

    return result;
}

DTime Project::totalTimeCurrentDay() const {
    vector<Task*>* tasks = _tasks;
    DTime result;
    for (vector<Task*>::iterator iter = tasks->begin(); iter != tasks->end(); iter++) {
        Task* task = *iter;
        if (task->childCount() == 0) {
            result.add(task->totalTimeCurrentDay());
        }
    }

    return result;
}

char* Project::toChar() {
    std::stringstream ss;
    ss << "project-name:" << *_name  << ";\n";
    if (_description != NULL) {
        ss << "project-description:" << *_description << ";\n";
    } else {
        ss << "project-description:;\n";
    }

    string ssOut =  ss.str();
    char* res = (char*)mmalloc(ssOut.size() + 1);
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

    delete(tasks);
    return res;
}

int Project::removeTask(Task* task) {
    qDebug("Removing task: %s", task->id()->c_str());

    vector<Task*> subTasks = task->children();
    // Removes all the children tasks
    for (vector<Task*>::iterator iterSub = subTasks.begin(); iterSub != subTasks.end(); iterSub++) {
        Task* tsk = *iterSub;
        removeTask(tsk);
    }
    subTasks.clear();

    vector<Task*>* tasks = _tasks;
    for (vector<Task*>::iterator iter = tasks->begin(); iter != tasks->end(); iter++) {
        Task* tsk = *iter;
        if (*tsk == *task) {
            tasks->erase(iter);
            break;
        }
    }

    Task* parentTask = task->parent();
    if (parentTask != NULL) {
        parentTask->removeChild(task);
    }

    std::map<string, Task*>::iterator iter = _tasksMap->find(*task->id());
    if (iter != _tasksMap->end()) {
        _tasksMap->erase(iter);
    }

    return 0;
}

string* Project::projectFileName() const {
    return _projectFileName;
}

void Project::setProjectFileName(string* projectFileName) {
    _projectFileName = projectFileName;
}

bool Project::operator ==(const Project& prj) const {
    return (prj._projectFileName->compare(*_projectFileName) == 0);
}

Calendar* Project::projectDefaultCalendar() const {
    return _projectDefaultCalendar;
}

int Project::taskCount() const {
    return _tasks->size();
}

std::vector<TaskLog*>* Project::logs() const {
    vector<TaskLog*>* logs = new vector<TaskLog*>();
    vector<Task*>* tsks = tasks();
    for (vector<Task*>::iterator iter = tsks->begin(); iter != tsks->end(); iter++) {
        Task* task = *iter;
        vector<TaskLog*>* taskLogs = task->logs();
        for (vector<TaskLog*>::iterator iterLog = taskLogs->begin(); iterLog != taskLogs->end(); iterLog++) {
            logs->push_back(*iterLog);
        }
        delete(taskLogs);
    }
    return logs;
}
