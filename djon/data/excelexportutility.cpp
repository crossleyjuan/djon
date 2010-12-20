#include "excelexportutility.h"
#include "util.h"
#include "data.h"

ExcelExportUtility::ExcelExportUtility(std::vector<Project*> projects)
{
    _projects = projects;
}

ExcelExportUtility::~ExcelExportUtility() {

}

ExcelExportUtility::ExcelExportUtility(const ExcelExportUtility &orig) {
    _projects = orig._projects;
}

void ExcelExportUtility::exportSubTasks(Excel& exc, Task* task, int& row, int indent) {
    std::vector<Task*>* subTasks = task->subTasks();
    for (std::vector<Task*>::iterator iterTask = subTasks->begin(); iterTask != subTasks->end(); iterTask++) {
        Task* subTask = *iterTask;
        std::description = subTask->shortDescription();
        char strI[indent];
        memset(strI, ' ', indent);
        exc(row, 1) = shortDescription;
        row++;
        if (subTask->childCount() > 0) {
            exportSubTasks(exc, subTask, row, 1);
        }
    }
    delete(subTasks);
}

void ExcelExportUtility::executeExport(std::string fileName, DateTime *logsFrom, DateTime *logsTo) {
    FILE *f = fopen(fileName.c_str(), "wb");

    Excel exc;

    int row = 0;
    for (std::vector<Project*>::iterator iterProj = _projects.begin(); iterProj != _projects.end(); iterProj++) {
        Project* project = *iterProj;

        exc(row, 0) = project->name()->c_str();
        row++;

        std::vector<Task*>* subTasks = project->subTasks();
        for (std::vector<Task*>::iterator iterTask = subTasks->begin(); iterTask != subTasks->end(); iterTask++) {
            Task* task = *iterTask;
            exc(row, 1) = (" " + task->shortDescription())->c_str();
            row++;
            if (task->childCount() > 0) {
                exportSubTasks(exc, task, row, 2);
            }
        }
        delete(subTasks);
    }

    exc.Write(f);
//    exc(0,0) = "Item1:";
//    exc(1,0) = "Item2:";
//    exc(2,0) = "Sum = ";
//    exc(2,0).setBorder(BORDER_LEFT | BORDER_TOP | BORDER_BOTTOM);
//    exc(2,0).setAlignament(ALIGN_CENTER);


//    exc(0,1) = 10;
//    exc(1,1) = 20;
//    miniexcel(1,1).setShaded(true);
//    miniexcel(2,1) = (double)miniexcel(0,1) + (double)miniexcel(1,1);
//    miniexcel(2,1).setBorder(BORDER_RIGHT | BORDER_TOP | BORDER_BOTTOM);

}
