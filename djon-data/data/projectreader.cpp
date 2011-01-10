#include "projectreader.h"

#include "project.h"
#include "task.h"

ProjectReader::ProjectReader(InputStream* input)
{
    _inputStream = input;
}

Project* ProjectReader::readProject() {
    char* id = _inputStream->readChars();
    Project* project = new Project();
    project->setId(new string(id));
    project->setName(_inputStream->readString());
    project->setDescription(_inputStream->readString());
    int taskCount = _inputStream->readInt();
    for (int x = 0; x < taskCount; x++) {
        Task* task = readTask(project);
        project->addTask(task);
    }
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
