#ifndef EXPORTUTILITY_H
#define EXPORTUTILITY_H

class Project;
#include <vector>

using namespace std;

class ExportUtility
{
public:
    ExportUtility(const vector<Project*> projects);
    ~ExportUtility();
    ExportUtility(const ExportUtility& utility);

    void executeExport();
private:
    vector<Project*> _projects;
};

#endif // EXPORTUTILITY_H
