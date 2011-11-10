// *********************************************************************************************************************
// file:    taskcombobox.cpp
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

#include "taskcombobox.h"

#include "data.h"


TaskComboBox::TaskComboBox(std::vector<Project*>* projects, QWidget* parent)
        : QComboBox(parent)
{
    _projects = projects;
    initialize();
}

void TaskComboBox::refresh(std::vector<Project*>* projects) {
    _projects = projects;
    initialize();
}

void TaskComboBox::initialize() {
    this->clear();
    int indent = 0;
    int id = 0;
    for (std::vector<Project*>::iterator iterProj = _projects->begin(); iterProj != _projects->end(); iterProj++) {
        Project* proj = *iterProj;
        this->addItem(QString(proj->name()->c_str()), ++id);
        _pointers.insert(pair<int, ComboItem*>(id, new ComboItem(id, false, proj)));
        // Only returns the direct subtasks
        std::vector<Task*>* tasks = proj->subTasks(new string(""));
        indent++;
        for (std::vector<Task*>::iterator iterTask = tasks->begin(); iterTask != tasks->end(); iterTask++) {
            Task* task = *iterTask;
            insertTask(task, id, indent);
        }
        delete(tasks);
    }
}

void TaskComboBox::insertTask(Task* task, int id, int& indent) {
    QString description = "+";
    for (int x = 0; x < indent; x++) {
        description.append(QString("--"));
    }
    description.append(QString(task->longDescription()->c_str()));
    this->addItem(description, ++id);
    _pointers.insert(pair<int, ComboItem*>(id, new ComboItem(id, true, task)));
    if (task->childCount() > 0) {
        vector<Task*> subTasks = task->children();
        indent++;
        for (std::vector<Task*>::iterator iterTask = subTasks.begin(); iterTask != subTasks.end(); iterTask++) {
            Task* task = *iterTask;
            insertTask(task, id, indent);
        }
        indent--;
    }

}
