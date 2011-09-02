#include "projectwriter.h"

#include "project.h"
#include "task.h"
#include "util.h"
#include "boost/crc.hpp"

ProjectWriter::ProjectWriter(OutputStream* output)
{
    _outStream = output;
}

void ProjectWriter::writeProject(const Project& project) {
    // Space to save the crc code
    //_outStream->writeLong(0L);
    _outStream->writeChars(project.id()->c_str(), project.id()->length());
    _outStream->writeChars(project.name()->c_str(), project.name()->length());
    _outStream->writeChars(project.description()->c_str(), project.description()->length());
    vector<Task*>* tasks = project.tasks();
    int taskCount = tasks->size();
    _outStream->writeInt(taskCount);
    _outStream->writeInt(project.type());
    for (vector<Task*>::iterator iterTask = tasks->begin(); iterTask != tasks->end(); iterTask++) {
        Task* task = *iterTask;
        writeTask(*task);
    }
    char* buffer = _outStream->buffer();

    qDebug("buffer: %s", buffer);
    boost::crc_32_type crc;
    crc.process_bytes(buffer, strlen(buffer));
    long valCrc = crc.checksum();
    qDebug("crc: %d", valCrc);
    delete(tasks);
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
