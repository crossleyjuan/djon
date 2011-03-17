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
