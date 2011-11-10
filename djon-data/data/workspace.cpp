// *********************************************************************************************************************
// file:    workspace.cpp
// author:  Juan Pablo Crossley (crossleyjuan@gmail.com)
// created: Oct/1/2010
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

#include "workspace.h"
#include "project.h"

Workspace::Workspace(std::string fileName)
{
    _projects = new std::vector<Project*>();
    this->_fileName = fileName;
}

Workspace::~Workspace() {
    delete(_projects);
}

Workspace::Workspace(const Workspace &workspace) {
    _projects = workspace._projects;
}

void Workspace::clear() {
    for (std::vector<Project*>::iterator iter = _projects->begin(); iter != _projects->end(); iter++) {
        delete(*iter);
    }
    _projects->clear();
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

std::string Workspace::fileName() const {
    return _fileName;
}
