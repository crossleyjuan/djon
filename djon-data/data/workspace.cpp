#include "workspace.h"

#include "project.h"

Workspace::Workspace()
{
    _projects = new std::vector<Project*>();
}

Workspace::~Workspace() {
    delete(_projects);
}

Workspace::Workspace(const Workspace &workspace) {
    _projects = workspace._projects;
}

void Workspace::addProject(Project *project) {
    _projects->push_back(project);
}

void Workspace::removeProject(Project *project) {
    for (std::vector<Project*>::iterator iter = _projects->begin(); iter != _projects->end(); iter++) {
        Project* item = *iter;
        if (project->id()->compare(*item->id()) == 0) {
            _projects->erase(iter);
            break;
        }
    }
}

const std::vector<Project*>* Workspace::projects() const {
    std::vector<Project*>* result = new std::vector<Project*>();
    for (std::vector<Project*>::iterator iter = _projects->begin(); iter != _projects->end(); iter++) {
        result->push_back(*iter);
    }
    return result;
}
