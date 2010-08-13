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
    const char* lastDir = getLastDir();

    string fileName = string(lastDir) + "/" + *project->projectFileName() + ".tsk";

    char* taskDefs = readFile(const_cast<char*>(fileName.c_str()));

    char* taskDef = strtok(taskDefs, "}}");
    while (taskDef != NULL) {
        char* tsk = strchr(taskDef, '{');
        if (tsk == NULL) {
            break;
        }
        string* staskDef = new string(tsk + 2);
        Task* task = new Task(project, staskDef);
        project->addTask(task);
        delete(staskDef);
        taskDef = strtok(NULL, "}}");
    }
}

void processTaskLog(Project* project, string* logDef) {
    hashmap* values = parseTextFormat(*logDef);

    std::string* taskLogId = new std::string(READ_ELEMENT(values, "log-id"));
    std::string* taskId = new std::string(READ_ELEMENT(values, "task-id"));
    DateTime* startDate = new DateTime(READ_ELEMENT(values, "log-start-time"));
    DateTime* endDate = new DateTime(READ_ELEMENT(values, "log-end-time"));
    std::string* logDescription = new string(READ_ELEMENT(values, "log-description"));

    TaskLog* log = new TaskLog();
    log->id = taskLogId;
    log->start = startDate;
    log->end = endDate;
    log->logDescription = logDescription;

    Task* task = project->task(*taskId);
    task->addLog(log);

    delete(values);
}

void loadTaskLogs(Project* project) {
    const char* lastDir = getLastDir();
    std::vector<char*> files;

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

        delete(slogDef);
        logDef = strtok(NULL, "}}");
    }
}

std::vector<Project*>* loadProjects() {
    const char* lastDir = getLastDir();
    std::vector<char*> files;

    std::vector<Project*>* projects = new std::vector<Project*>();

    int res = getdir(lastDir, files, std::string("djon").c_str());
    if (res == 0) {
        for (vector<char*>::iterator it = files.begin(); it != files.end(); it++) {
            char* fileName = (*it);
            stringstream ss;
            ss << lastDir;
            ss << "/";
            ss << fileName;
            char* projectDef = readFile(const_cast<char*>(ss.str().c_str()));

            string projectFileName(fileName);
            unsigned int dot = projectFileName.find_last_of('.');
            if (dot != projectFileName.npos) {
                projectFileName = projectFileName.substr(0, dot);
            }
            Project* project = new Project(projectDef);
            project->setProjectFileName(new string(projectFileName));
            projects->push_back(project);
            loadTasks(project);
            loadTaskLogs(project);
        }
    }
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

            hashmap* conf = parseTextFormat(string(readFile(const_cast<char*>(fileName.c_str()))));

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
    string* current = new string(readFile(const_cast<char*>(fileName.str().c_str())));

    int posStart = current->find(string("{{\ntask-id:") + *task->id() + ";");
    int posEnd = current->find(string("}}\n"), posStart) + 3;

    int size = posEnd - posStart;
    string newFile = current->replace(posStart, size, ssTaskDef.str());

    int res = writeFile(fileName.str(), newFile, false);

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
    ssTaskLogDef << "}}\n";

    string* projName = task->project()->projectFileName();
    const char* lastDir = getLastDir();
    stringstream fileName;
    fileName << lastDir << "/" << *projName << ".log";
    string* current = new string(readFile(const_cast<char*>(fileName.str().c_str())));

    int posStart = current->find(string("{{\nlog-id:") + *taskLog->id + ";");
    int posEnd = current->find(string("}}\n"), posStart) + 3;

    int size = posEnd - posStart;
    string newFile = current->replace(posStart, size, ssTaskLogDef.str());

    int res = writeFile(fileName.str(), newFile, false);

    return res;
}

int deleteTaskLog(Task* task, TaskLog* taskLog) {
    string* projName = task->project()->projectFileName();
    const char* lastDir = getLastDir();
    stringstream fileName;
    fileName << lastDir << "/" << *projName << ".log";
    string* current = new string(readFile(const_cast<char*>(fileName.str().c_str())));

    string lookingFor = string("{{\nlog-id:") + *taskLog->id + ";";
    int posStart = current->find(lookingFor);
    int posEnd = current->find(string("}}\n"), posStart) + 3;

    int size = posEnd - posStart;
    string newFile = current->replace(posStart, size, string(""));

    int res = writeFile(fileName.str(), newFile, false);

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
    string* current = new string(readFile(const_cast<char*>(fileName.str().c_str())));

    int posStart = current->find(string("{{\ntask-id:") + *task->id() + ";");
    int posEnd = current->find(string("}}\n"), posStart) + 3;

    if (posStart <= 0) {
        setLastError(3, "the task id: %s was not found in the task file, and will not be updated.", task->id()->c_str());
        return 1;
    } else {
        int size = posEnd - posStart;
        string newFile = current->replace(posStart, size, string(""));

        res += writeFile(fileName.str(), newFile, false);

        return res;
    }
}
