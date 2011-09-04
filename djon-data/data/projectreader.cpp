#include "projectreader.h"

#include "project.h"
#include "task.h"
#include "util.h"
#include "settings.h"
#include "boost/crc.hpp"
#include <sstream>
#include <iostream>
#include <stdlib.h>

ProjectReader::ProjectReader(InputStream* input)
{
    _inputStream = input;
}

bool isValidVersionNumber(char* version) {
    bool isVersion = true;
    for (int x = 0; x < strlen(version); x++) {
        isVersion = isVersion && (isalnum(version[x]) || version[x] == '.');
    }
    return isVersion;
}

const char* getLastDir() {
    return getSettings()->lastProjectDir().c_str();
}

void loadTasks(Project* project) {
    qDebug("loadTasks(Project* project)");

    string fileName = project->projectFileName()->substr(0, project->projectFileName()->length() - 4) + "tsk";

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

    string fileName = project->projectFileName()->substr(0, project->projectFileName()->length() - 4) + "log";

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

//Project* readProject11() {
//    char* projDef = readFile((char*)fileName.c_str());
//    Project* project = new Project(string(projDef));

//    project->setProjectFileName(new string(fileName));
//    projects->push_back(project);
//    loadTasks(project);
//    if (errorOcurred()) {
//        return NULL;
//    }
//    loadTaskLogs(project);
//    if (errorOcurred()) {
//        return NULL;
//    }
//    free(projDef);

//}

Project* ProjectReader::readProject() {
    char* version = _inputStream->readChars(13);
    Version* fileVersion;
    Version format1 = Version("1.1.201011110");
    Version format2 = Version("1.2.20110725");
    if (!isValidVersionNumber(version)) {
        fileVersion = new Version("1.1.201011110");
    } else {
        fileVersion = new Version(string(version));
    }

    Project* project;
    if (*fileVersion == format1) {
        const char* projDef = _inputStream->readFull();
        project = new Project(string(projDef));
        project->setId(uuid());
        project->setProjectFileName(new string(_inputStream->fileName()));
        loadTasks(project);
        loadTaskLogs(project);
    } else {
        if (*fileVersion > format2) {
            long currentPos = _inputStream->currentPos();
            long crc = _inputStream->readLong();
            currentPos = _inputStream->currentPos();
            long crcCalculated = _inputStream->crc32();
            if (crcCalculated != crc) {
                setLastError(10, "The file %s has an invalid checksum. The file is corrupted and can not be loaded.", _inputStream->fileName().c_str());
                return NULL;
            }
            _inputStream->seek(currentPos);

        }
        char* id = _inputStream->readChars();
        project = new Project();
        project->setId(new string(id));
        project->setName(_inputStream->readString());
        project->setDescription(_inputStream->readString());
        project->setProjectFileName(new string(_inputStream->fileName()));
        int taskCount = _inputStream->readInt();
        if (*fileVersion >= format2) {
            int type = _inputStream->readInt();
            project->setType((Project::PROJECTTYPE)type);
        } else {
            project->setType(Project::NORMAL);
        }
        for (int x = 0; x < taskCount; x++) {
            Task* task = readTask(project);
            project->addTask(task);
        }
    }
    delete(fileVersion);
    return project;
}

Task* ProjectReader::readTask(Project* project) {
    Task* task = new Task(project);
    task->setId(_inputStream->readString());
    task->setShortDescription(_inputStream->readString());
    task->setLongDescription(_inputStream->readString());
    string* startDate = _inputStream->readString();
    task->setStartDate(new DateTime(*startDate));
    delete(startDate);
    string* endDate = _inputStream->readString();
    task->setEndDate(new DateTime(*endDate));
    delete(endDate);
    task->setDuration(Duration(string(_inputStream->readChars())));
    task->setStatus(_inputStream->readString());
    task->setTemplateName(_inputStream->readString());
    int logs = _inputStream->readInt();
    for (int i = 0; i < logs; i++) {
        TaskLog* log = readTaskLog(task);
        task->addLog(log);
    }
    return task;
}

TaskLog* ProjectReader::readTaskLog(Task* task) {
    TaskLog* log = new TaskLog();
    log->id = _inputStream->readString();
    log->logDescription = _inputStream->readString();
    log->start = new DateTime(*_inputStream->readString());
    log->end = new DateTime(*_inputStream->readString());
    log->lastLap = NULL;
    string* lastLap = _inputStream->readString();
    if (lastLap->length() != 0) {
        log->lastLap = new DateTime(*lastLap);
    }

    return log;
}
