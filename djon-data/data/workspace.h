#ifndef WORKSPACE_H
#define WORKSPACE_H

#include "vector"
#include <string>

class Project;

class Workspace
{
public:
    Workspace(std::string fileName);
    virtual ~Workspace();
    Workspace(const Workspace& workspace);

    void addProject(Project* project);
    void removeProject(Project* project);
    void clear();
    std::string fileName() const;

    const std::vector<Project*>* projects() const;
private:
    std::vector<Project*>* _projects;
    std::string _fileName;
};

#endif // WORKSPACE_H
