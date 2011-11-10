// *********************************************************************************************************************
// file:    excelexportutility.cpp
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

#include "excelexportutility.h"
#include "util.h"
#include "data.h"
#include <sstream>
#include <string>

ExcelExportUtility::ExcelExportUtility(std::vector<Project*> projects)
{
    _projects = projects;
}

ExcelExportUtility::~ExcelExportUtility() {

}

ExcelExportUtility::ExcelExportUtility(const ExcelExportUtility &orig) {
    _projects = orig._projects;
}

void ExcelExportUtility::exportSubTasks(Excel& excel, std::vector<Task*> subTasks, int& row, int indent) {
    for (std::vector<Task*>::iterator iterTask = subTasks.begin(); iterTask != subTasks.end(); iterTask++) {
        Task* subTask = *iterTask;
        std::string description = *subTask->shortDescription();
        char strI[indent + 1];
        memset(strI, 0, indent + 1);
        memset(strI, ' ', indent);
        std::stringstream ss;
        ss << strI;
        ss << description;
        description = ss.str();
        excel(row, 0) = description.c_str();
        row++;
        if (subTask->childCount() > 0) {
            exportSubTasks(excel, subTask->children(), row, 1);
        }
    }
}

void ExcelExportUtility::executeExport(std::string fileName, DateTime *logsFrom, DateTime *logsTo) {
//    FILE *f = fopen(fileName.c_str(), "wb");

    Excel exc;

    int row = 0;
    for (std::vector<Project*>::iterator iterProj = _projects.begin(); iterProj != _projects.end(); iterProj++) {
        Project* project = *iterProj;

        exc(row, 0) = project->name()->c_str();
        row++;

        std::vector<Task*>* subTasks = project->subTasks();
        exportSubTasks(exc, *subTasks, row, 0);
    }

    exc.write();
}
