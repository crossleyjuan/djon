// *********************************************************************************************************************
// file:    exportutility.cpp
// author:  Juan Pablo Crossley (crossleyjuan@gmail.com)
// created: Dec/23/2010
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

#include "exportutility.h"
#include "data.h"
#include <fstream>
#include <sstream>

ExportUtility::ExportUtility(vector<Project*> projects) {
    _projects = projects;
}

ExportUtility::~ExportUtility() {

}

ExportUtility::ExportUtility(const ExportUtility& utility) {
    _projects = utility._projects;
}

void ExportUtility::executeExport(std::string fileName, DateTime* logsFrom, DateTime* logsTo) {
    ofstream result(fileName.c_str());

    result << "<?xml version=\"1.0\" encoding=\"windows-1252\"?>\n";
    result << "<projects>\n";
    for (vector<Project*>::iterator iterProj = _projects.begin(); iterProj != _projects.end(); iterProj++) {
        Project* proj = *iterProj;
        result << "<project ";
        result << "name=\"" << *proj->name() << "\" ";
        result << "totalTime=\"" << proj->totalTime().toChar() << "\" ";

        DTime totalProjectLogTime;
        stringstream ssTasks;
        ssTasks << "<tasks>\n";
        vector<Task*>* tasks = proj->tasks();
        bool includeTask = false;
        for (vector<Task*>::iterator iterTask = tasks->begin(); iterTask != tasks->end(); iterTask++) {
            Task* task = *iterTask;
            stringstream ssTask;
            ssTask << "<task ";
            ssTask << "number=\"" << *task->id() << "\" ";
            ssTask << "name=\"" << *task->shortDescription() << "\" ";
            ssTask << "start=\"" << task->startDate()->toQDateTime().toString(Qt::ISODate).toStdString() << "\" ";
            ssTask << "end=\"" << task->endDate()->toQDateTime().toString(Qt::ISODate).toStdString() << "\" ";
            ssTask << "duration=\"" << task->duration().toChar() << "\" ";
            ssTask << "timelength=\"" << task->totalTime().toChar() << "\" ";

            stringstream periods;
            vector<TaskLog*>* logs = task->logs();
            DTime totalLogTime;
            periods << "<periods>\n";
            for (vector<TaskLog*>::iterator iterLog = logs->begin(); iterLog != logs->end(); iterLog++) {
                TaskLog* log = *iterLog;
                bool print = true;
                if (logsFrom) {
                    if (*log->start < *logsFrom) {
                        print = false;
                    }
                }
                if (logsTo) {
                    if (*log->end > *logsTo) {
                        print = false;
                    }
                }
                if (print) {
                    includeTask = true;
                    periods << "<period ";
                    periods << "id=\"" << *log->id << "\" ";
                    periods << "start=\"" << log->start->toQDateTime().toString(Qt::ISODate).toStdString() << "\" ";
                    periods << "finish=\"" << log->end->toQDateTime().toString(Qt::ISODate).toStdString() << "\" ";
                    periods << "description=\"" << *log->logDescription << "\" ";
                    periods << "timelength=\"" << log->totalTime().toChar() << "\" />\n";
                    int logSecs = *log->end - *log->start;
                    totalLogTime.add(logSecs);
                    if (logSecs < 0) {
                        qDebug("Inconsistent log. Id: %s, Task Id: %s, Project: %s", log->id->c_str(), task->id()->c_str(), task->project()->name()->c_str());
                    }
                    totalProjectLogTime.add(logSecs);
                }
            }
            periods << "</periods>\n";
            ssTask << "logTime=\"" << totalLogTime.toChar() << "\">\n";
            ssTask << periods.str();
            ssTask << "</task>\n";
            if (includeTask) {
                ssTasks << ssTask.str();
            }
            delete(tasks);
        }
        ssTasks << "</tasks>\n";
        result << " logTime=\"" << totalProjectLogTime.toChar() << "\" >\n";
        result << ssTasks.str();
        result << "</project>\n";
    }
    result << "</projects>\n";
    result.flush();
    result.close();
}
