#ifndef TASKITEM_H
#define TASKITEM_H

 #include <QList>
 #include <QVariant>
#include "data.h"

enum ELEMENT_TYPE {
    NONE,
    TASK,
    PROJECT
};

 class TaskItem
 {
 public:
     TaskItem(const QList<QVariant> &data, TaskItem *parent = 0);
     TaskItem(Project* project, Task* task, TaskItem *parent = 0);
     TaskItem(Project* project, TaskItem *parent = 0);
     ~TaskItem();

     void appendChild(TaskItem *child);

     TaskItem *child(int row);
     int childCount() const;
     int columnCount() const;
     QVariant data(int column) const;
     int row() const;
     TaskItem *parent();
     Task* task();
     Project* project();

 private:
     QList<TaskItem*> childItems;
     QList<QVariant> itemData;
     TaskItem *parentItem;
     Task* _task;
     Project* _project;
     ELEMENT_TYPE _type; // true if the taskitem represents a task, false if it's a project
 };
#endif // TASKITEM_H
