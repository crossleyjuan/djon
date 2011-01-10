#include "projectwriter.h"

#include "project.h"
#include "task.h"

ProjectWriter::ProjectWriter(OutputStream* output)
{
    _outStream = output;
}

void ProjectWriter::writeProject(const Project& project) {
    _outStream->writeChars(project.id()->c_str(), project.id()->length());
    _outStream->writeChars(project.name()->c_str(), project.name()->length());
    _outStream->writeChars(project.description()->c_str(), project.description()->length());
    vector<Task*>* tasks = project.tasks();
    int taskCount = tasks->size();
    _outStream->writeInt(taskCount);
    for (vector<Task*>::iterator iterTask = tasks->begin(); iterTask != tasks->end(); iterTask++) {
        Task* task = *iterTask;
        writeTask(*task);
    }
}

void ProjectWriter::writeTask(const Task &task) {
    _outStream->writeChars(task.id()->c_str(), task.id()->length());
    _outStream->writeString(task.shortDescription());
    _outStream->writeString(task.longDescription());
    char* startDate = task.startDate()->toChar();
    _outStream->writeChars(startDate, strlen(startDate));
    char* endDate = task.endDate()->toChar();
    _outStream->writeChars(endDate, strlen(endDate));
    char* duration = task.duration().toChar();
    _outStream->writeChars(duration, strlen(duration));
    _outStream->writeString(task.status());
    _outStream->writeString(task.templateName());

    vector<TaskLog*>* logs = task.logs();
    _outStream->writeInt(logs->size());
    for (vector<TaskLog*>::iterator iter = logs->begin(); iter != logs->end(); iter++) {
        TaskLog* log = *iter;
        writeTaskLog(*log);
    }
}

void ProjectWriter::writeTaskLog(const TaskLog &taskLog) {
    _outStream->writeChars(taskLog.id->c_str(), taskLog.id->length());
    _outStream->writeString(taskLog.logDescription);
    char* startDate = taskLog.start->toChar();
    _outStream->writeChars(startDate, strlen(startDate));
    char* endDate = taskLog.end->toChar();
    _outStream->writeChars(endDate, strlen(endDate));
    string lastLap("");
    if (taskLog.lastLap != NULL) {
        lastLap = string(taskLog.lastLap->toChar());
    }
    _outStream->writeString(&lastLap);
}
