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
    }
}

void TaskComboBox::insertTask(Task* task, int id, int indent) {
    this->addItem(QString(task->longDescription()->c_str()), ++id);
    _pointers.insert(pair<int, ComboItem*>(id, new ComboItem(id, true, task)));
    vector<Task*>* subTasks = task->subTasks();
    if (subTasks->size() > 0) {
        for (std::vector<Task*>::iterator iterTask = subTasks->begin(); iterTask != subTasks->end(); iterTask++) {
            Task* task = *iterTask;
            insertTask(task, id, indent);
        }
    }

}
