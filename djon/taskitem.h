#ifndef TASKITEM_H
#define TASKITEM_H

#include <QList>
#include <QVariant>
#include "data.h"

enum ELEMENT_TYPE {
    BLANK,
    NONE,
    TASK,
    PROJECT,
    SUMMARY
};

class TaskItem
{
public:
    TaskItem(const QList<QVariant> &data, TaskItem *parent = 0);
    TaskItem(Project* project, Task* task, TaskItem *parent = 0);
    TaskItem(Project* project, TaskItem *parent = 0);
    TaskItem(std::vector<Project*> projects, TaskItem *parent = 0);
    ~TaskItem();

    void appendChild(TaskItem *child);
    void removeChild(TaskItem *child);
    void clear();

    TaskItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column, int role) const;
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
    std::vector<Project*> _allprojects;
    ELEMENT_TYPE _type;
};
#endif // TASKITEM_H
