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
