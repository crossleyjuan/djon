#ifndef EXPORTUTILITY_H
#define EXPORTUTILITY_H

class Project;
#include <vector>
#include <string>

using namespace std;

class ExportUtility
{
public:
    ExportUtility(const vector<Project*> projects);
    ~ExportUtility();
    ExportUtility(const ExportUtility& utility);

    void executeExport(std::string fileName);
private:
    vector<Project*> _projects;
};

#endif // EXPORTUTILITY_H
