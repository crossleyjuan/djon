// *********************************************************************************************************************
// file:    projectwriter.cpp
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

#include "projectwriter.h"

#include "project.h"
#include "task.h"
#include "util.h"

ProjectWriter::ProjectWriter(OutputStream* output)
{
    _outStream = output;
}

void ProjectWriter::writeProject(const Project& project) {
    // Space to save the crc code
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
