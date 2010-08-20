#ifndef EXPORTUTILITY_H
#define EXPORTUTILITY_H

class Project;
class DateTime;
#include <vector>
#include <string>

using namespace std;

class ExportUtility
{
public:
    ExportUtility(const vector<Project*> projects);
    ~ExportUtility();
    ExportUtility(const ExportUtility& utility);

    void executeExport(std::string fileName, DateTime* logsFrom = 0, DateTime* logsTo = 0);

private:
    vector<Project*> _projects;
};

#endif // EXPORTUTILITY_H
