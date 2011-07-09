#ifndef WORKSPACE_H
#define WORKSPACE_H

#include "vector"

class Project;

class Workspace
{
public:
    Workspace();
    virtual ~Workspace();
    Workspace(const Workspace& workspace);

    void addProject(Project* project);
    void removeProject(Project* project);

    const std::vector<Project*>* projects() const;
private:
    std::vector<Project*>* _projects;
};

#endif // WORKSPACE_H
