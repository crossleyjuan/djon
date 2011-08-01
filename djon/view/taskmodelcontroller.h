#ifndef TASKMODELCONTROLLER_H
#define TASKMODELCONTROLLER_H

#include "TaskModel.h"
#include "vector"

class Task;

class TaskModelController
{
public:
    TaskModelController();

    void add(TaskModel* model);
    void remove(TaskModel* model);

    void addTask(Task* task);

private:
    std::vector<TaskModel*> _models;
};

#endif // TASKMODELCONTROLLER_H
