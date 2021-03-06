#ifndef EXCELEXPORTUTILITY_H
#define EXCELEXPORTUTILITY_H

#include <vector>
#include <string>
#include "excel.h"

class Project;
class Task;
class DateTime;

class ExcelExportUtility
{
public:
    ExcelExportUtility(std::vector<Project*> projects);
    ~ExcelExportUtility();
    ExcelExportUtility(const ExcelExportUtility& orig);

    void executeExport(std::string fileName, DateTime* logsFrom = 0, DateTime* logsTo = 0);

private:
    void exportSubTasks(Excel& excel, std::vector<Task*> subTasks, int& row, int indent);

    std::vector<Project*> _projects;
};

#endif // EXCELEXPORTUTILITY_H
