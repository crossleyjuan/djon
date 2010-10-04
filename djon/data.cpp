#include "data.h"
#include "util.h"
#include "config.h"
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;

const char* getLastDir() {
    return readConfValue("last-project-dir", std::string("").c_str());
}

void loadTasks(Project* project) {
    qDebug("loadTasks(Project* project)");
    const char* lastDir = getLastDir();

    string fileName = string(lastDir) + "/" + *project->projectFileName() + ".tsk";

    char* taskDefs = readFile(const_cast<char*>(fileName.c_str()));

    std::vector<string*>* taskDef = tokenizer(taskDefs, "}}");
    for (std::vector<string*>::iterator iter = taskDef->begin(); iter != taskDef->end(); iter++) {
        string* staskDef = *iter;
        if (staskDef->find("{{") != std::string::npos) {
            string tsk = staskDef->substr(staskDef->find("{{") + 2);
            Task* task = new Task(project, tsk);
            project->addTask(task);
            if (errorOcurred()) {
                return;
            }
        }
    }
    free(taskDefs);
    qDebug("out loadTasks(Project* project)");
}

void processTaskLog(Project* project, string* logDef) {
    qDebug("processTaskLog(Project* project, string* logDef)");
    hashmap* values = parseTextFormat(*logDef);

    std::string* taskLogId = new std::string(READ_ELEMENT(values, "log-id"));
    std::string* taskId = new std::string(READ_ELEMENT(values, "task-id"));
    DateTime* startDate = new DateTime(READ_ELEMENT(values, "log-start-time"));
    DateTime* endDate = new DateTime(READ_ELEMENT(values, "log-end-time"));
    std::string* logDescription = new string(READ_ELEMENT(values, "log-description"));
    DateTime* lap = NULL;
    string logLap = READ_ELEMENT(values, "log-lap");

    if (logLap.length() > 0) {
        lap = new DateTime(logLap);
    }
    TaskLog* log = new TaskLog();
    log->id = taskLogId;
    log->start = startDate;
    log->end = endDate;
    log->lastLap = lap;
    log->logDescription = logDescription;

    Task* task = project->task(*taskId);
    if (task == NULL) {
        setLastError(5, "The tasklog %s refers an invalid task %s.", taskLogId->c_str(), taskId->c_str());
        return;
    }
    task->addLog(log);

    delete(values);
    qDebug("out processTaskLog(Project* project, string* logDef)");
}

void loadTaskLogs(Project* project) {
    qDebug("loadTaskLogs(Project* project)");
    const char* lastDir = getLastDir();

    string fileName = string(lastDir) + "/" + *project->projectFileName() + ".log";

    char* logDefs = readFile(const_cast<char*>(fileName.c_str()));

    char* logDef = strtok(logDefs, "}}");
    while (logDef != NULL) {
        char* log = strchr(logDef, '{');
        if (log == NULL) {
            break;
        }
        string* slogDef = new string(log + 2);
        processTaskLog(project, slogDef);

        if (errorOcurred()) {
            return;
        }
        delete(slogDef);
        logDef = strtok(NULL, "}}");
    }
    free(logDefs);
    qDebug("out loadTaskLogs(Project* project)");
}

std::vector<Project*>* loadProjects() {
    qDebug("loadProjects()");
    const char* lastDir = getLastDir();

    std::vector<Project*>* projects = new std::vector<Project*>();

    const char* openProjects = readConfValue("open-projects", "");
    vector<string*>* prjs = split(string(openProjects), string(","));

    for (vector<string*>::iterator it = prjs->begin(); it != prjs->end(); it++) {
        string* fileName = (*it);
        stringstream ss;
        ss << lastDir;
        ss << "/";
        ss << *fileName;
        ss << ".djon";
        string fullPath = ss.str();
        char* projectDef = readFile(const_cast<char*>(fullPath.c_str()));

        Project* project = new Project(projectDef);
        project->setProjectFileName(fileName);
        projects->push_back(project);
        loadTasks(project);
        if (errorOcurred()) {
            return NULL;
        }
        loadTaskLogs(project);
        if (errorOcurred()) {
            return NULL;
        }
        free(projectDef);
    }
    qDebug("out loadProjects()");
    return projects;
}


vector<Template*>* m_templates;

vector<Template*>* readTemplates() {
    if (m_templates != NULL) {
        return m_templates;
    }
    m_templates = new vector<Template*>();
    string* home = getHomeDir();
    string path = *home + "/.djon/templates/";

    vector<char*> files;
    int res = getdir(const_cast<char*>(path.c_str()), files, "tpl");

    if (res == 0) {
        for (vector<char*>::iterator it = files.begin(); it != files.end(); it++) {
            string fileName = path + std::string(*it);

            char* tempDef = readFile(const_cast<char*>(fileName.c_str()));
            hashmap* conf = parseTextFormat(string(tempDef));

            string* templateName = new std::string(READ_ELEMENT(conf, "template-name"));
            string* templateDescription = new std::string(READ_ELEMENT(conf, "template-description"));
            string statusList = READ_ELEMENT(conf, "status");
            string subTaskList = READ_ELEMENT(conf, "subtasks");

            vector<string*>* vecStatus = split(statusList, ",");
            vector<string*>* vecSubTaskList = split(subTaskList, ",");

            Template* tpl = new Template(templateName, templateDescription, vecStatus, vecSubTaskList);
            if ((*templateName)[templateName->length() - 1] == '*') {
                m_templates->insert(m_templates->begin(), tpl);
            } else {
                m_templates->push_back(tpl);
            }
            free(tempDef);
        }
    }

    delete(home);
    return m_templates;
}

Template* readTemplate(const string& name) {
    vector<Template*>* templates = readTemplates();
    for (vector<Template*>::iterator it = templates->begin(); it != templates->end(); it++) {
        Template* t = *it;
        if (t->name()->compare(name) == 0) {
            return t;
        }
    }
    return NULL;
}

int createTask(Task* task) {
    std::stringstream ssTaskDef;
    const char* taskDef = task->toChar();
    ssTaskDef << "{{\n";
    ssTaskDef << taskDef;
    ssTaskDef << "}}\n";

    string* projName = task->project()->projectFileName();
    const char* lastDir = getLastDir();
    stringstream fileName;
    fileName << lastDir << "/" << *projName << ".tsk";
    int res = writeFile(fileName.str(), ssTaskDef.str(), true);
    return res;
}

int updateTask(Task* task) {
    std::stringstream ssTaskDef;
    const char* taskDef = task->toChar();
    ssTaskDef << "{{\n";
    ssTaskDef << taskDef;
    ssTaskDef << "}}\n";

    string* projName = task->project()->projectFileName();
    const char* lastDir = getLastDir();
    stringstream fileName;
    fileName << lastDir << "/" << *projName << ".tsk";
    char* ccurrent = readFile(const_cast<char*>(fileName.str().c_str()));
    string* current = new string(ccurrent);

    int posStart = current->find(string("{{\ntask-id:") + *task->id() + ";");
    int posEnd = current->find(string("}}\n"), posStart) + 3;

    int size = posEnd - posStart;
    string newFile = current->replace(posStart, size, ssTaskDef.str());

    int res = writeFile(fileName.str(), newFile, false);

    free(ccurrent);
    delete(current);
    return res;
}

int createProject(Project* project) {
    std::string projDef(project->toChar());

    string* projName = project->name();

    const char* lastDir = getLastDir();
    stringstream fileName;
    fileName << lastDir << "/" << *projName << ".djon";

    int res = writeFile(fileName.str(), projDef, false);

    project->setProjectFileName(projName);

    addProject(projName->c_str());

    return res;
}

int updateProject(Project* project) {
    std::string projDef(project->toChar());

    string* projName = project->projectFileName();
    const char* lastDir = getLastDir();
    stringstream fileName;
    fileName << lastDir << "/" << *projName << ".djon";

    int res = writeFile(fileName.str(), projDef, false);

    return res;
}

int createTaskLog(Task* task, TaskLog* taskLog) {
    std::stringstream ssTaskLogDef;

    ssTaskLogDef << "{{\n";
    ssTaskLogDef << "log-id:" << *taskLog->id << ";\n";
    ssTaskLogDef << "task-id:" << *task->id() << ";\n";
    ssTaskLogDef << "log-start-time:" << taskLog->start->toChar() << ";\n";
    ssTaskLogDef << "log-end-time:" << taskLog->end->toChar() << ";\n";
    if (taskLog->logDescription != NULL) {
        ssTaskLogDef << "log-description:" << *taskLog->logDescription << "\n";
    }
    if (taskLog->lastLap != NULL) {
        ssTaskLogDef << "log-lap:" << *taskLog->lastLap->toChar() << "\n";
    }
    ssTaskLogDef << "}}";

    string* projName = task->project()->projectFileName();
    const char* lastDir = getLastDir();
    stringstream fileName;
    fileName << lastDir << "/" << *projName << ".log";
    int res = writeFile(fileName.str(), ssTaskLogDef.str(), true);
    return res;
}

int updateTaskLog(Task* task, TaskLog* taskLog) {
    std::stringstream ssTaskLogDef;

    ssTaskLogDef << "{{\n";
    ssTaskLogDef << "log-id:" << *taskLog->id << ";\n";
    ssTaskLogDef << "task-id:" << *task->id() << ";\n";
    ssTaskLogDef << "log-start-time:" << taskLog->start->toChar() << ";\n";
    ssTaskLogDef << "log-end-time:" << taskLog->end->toChar() << ";\n";
    if (taskLog->logDescription != NULL) {
        ssTaskLogDef << "log-description:" << *taskLog->logDescription << "\n";
    }
    if (taskLog->lastLap != NULL) {
        ssTaskLogDef << "log-lap:" << *taskLog->lastLap->toChar() << "\n";
    }
    ssTaskLogDef << "}}\n";

    string* projName = task->project()->projectFileName();
    const char* lastDir = getLastDir();
    stringstream fileName;
    fileName << lastDir << "/" << *projName << ".log";
    char* ccurrent = readFile(const_cast<char*>(fileName.str().c_str()));
    string* current = new string(ccurrent);

    int posStart = current->find(string("{{\nlog-id:") + *taskLog->id + ";");
    int posEnd = current->find(string("}}\n"), posStart) + 3;

    int size = posEnd - posStart;
    string newFile = current->replace(posStart, size, ssTaskLogDef.str());

    int res = writeFile(fileName.str(), newFile, false);

    free(ccurrent);
    delete(current);
    return res;
}

int deleteTaskLog(Task* task, TaskLog* taskLog) {
    string* projName = task->project()->projectFileName();
    const char* lastDir = getLastDir();
    stringstream fileName;
    fileName << lastDir << "/" << *projName << ".log";
    char* ccurrent = readFile(const_cast<char*>(fileName.str().c_str()));
    string* current = new string(ccurrent);

    string lookingFor = string("{{\nlog-id:") + *taskLog->id + ";";
    int posStart = current->find(lookingFor);
    int posEnd = current->find(string("}}\n"), posStart) + 3;

    int size = posEnd - posStart;
    string newFile = current->replace(posStart, size, string(""));

    int res = writeFile(fileName.str(), newFile, false);

    free(ccurrent);
    delete(current);
    return res;
}

int deleteTask(Task* task) {
    int res = 0;
    std::vector<TaskLog*>* logs = task->logs();
    for (std::vector<TaskLog*>::iterator iterLog = logs->begin(); iterLog != logs->end(); iterLog++) {
        TaskLog* log = *iterLog;
        int resLog = deleteTaskLog(task, log);
        res += resLog;
    }

    std::stringstream ssTaskDef;
    const char* taskDef = task->toChar();
    ssTaskDef << "{{\n";
    ssTaskDef << taskDef;
    ssTaskDef << "}}\n";

    string* projName = task->project()->projectFileName();
    const char* lastDir = getLastDir();
    stringstream fileName;
    fileName << lastDir << "/" << *projName << ".tsk";
    char* ccurrent = readFile(const_cast<char*>(fileName.str().c_str()));
    string* current = new string(ccurrent);

    int posStart = current->find(string("{{\ntask-id:") + *task->id() + ";");
    int posEnd = current->find(string("}}\n"), posStart) + 3;

    if (posStart == std::string::npos) {
        setLastError(3, "the task id: %s was not found in the task file, and will not be updated.", task->id()->c_str());
        res = 1;
    } else {
        int size = posEnd - posStart;
        string newFile = current->replace(posStart, size, string(""));

        res += writeFile(fileName.str(), newFile, false);
    }
    free(ccurrent);
    free(current);
    return res;
}

void addProject(const char* fileName) {
    string* openProjects = new string(readConfValue("open-projects", ""));
    if (openProjects->find(fileName) == -1) {
        if (openProjects->length() > 0) {
            openProjects->append(",");
        } else {
            openProjects = new string("");
        }
        openProjects->append(fileName);
        writeConfValue("open-projects", *openProjects);
    }
}

void removeProject(const char* fileName) {
    string* openProjects = new string(readConfValue("open-projects", ""));
    vector<string*>* projects = split(*openProjects, ",");
    vector<string*> newProjects;
    for (vector<string*>::iterator iter = projects->begin(); iter != projects->end(); iter++) {
        string* project = *iter;
        if (project->compare(fileName) != 0) {
            newProjects.push_back(project);
        }
    }
    string result;
    for (vector<string*>::iterator i = newProjects.begin(); i != newProjects.end(); ) {
        string* file = *i;
        result = result.append(*file);
        i++;
        if (i != newProjects.end()) {
            result = result.append(",");
        }
    }
    writeConfValue("open-projects", result);
}

Project* searchProject(vector<Project*> projects, string name) {
    vector<Project*>::iterator iterProj = projects.begin();
    for (; iterProj != projects.end(); iterProj++) {
        Project* prj = *iterProj;
        if (prj->name()->compare(name) == 0) {
            return prj;
            break;
        }
    }
    return NULL;
}
