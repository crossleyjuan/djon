#include "taskitem.h"
#include <QStringList>

 TaskItem::TaskItem(Project* project, Task* task, TaskItem *parent) {
     parentItem = parent;
     _task = task;
    _project = project;
    _type = TASK;
 }

 TaskItem::TaskItem(const QList<QVariant> &data, TaskItem *parent) {
     parentItem = parent;
     itemData = data;
     _task = NULL;
    _project = NULL;
    _type = NONE;
 }

 TaskItem::TaskItem(Project* project, TaskItem *parent) {
    parentItem = parent;
    _project = project;
     _task = NULL;
    _type = PROJECT;
 }

 TaskItem::~TaskItem()
 {
     qDeleteAll(childItems);
 }

 void TaskItem::appendChild(TaskItem *item)
 {
     childItems.append(item);
 }

 TaskItem *TaskItem::child(int row)
 {
     return childItems.value(row);
 }

 int TaskItem::childCount() const
 {
     return childItems.count();
 }

 int TaskItem::columnCount() const
 {
     return 3;
 }

 QVariant TaskItem::data(int column) const
 {
     switch(column) {
     case 0:
         if (_type == PROJECT) {
             return QString(_project->name()->c_str());
         }
         if (_type == TASK) {
             return QString(_task->shortDescription()->c_str());
         }
         if (_type == NONE) {
             return itemData.at(column);
         }
     case 1:
         if (_type == PROJECT) {
             return *_project->totalTime()->toQTime();
         }
         if (_type == TASK) {
             return *_task->totalTime()->toQTime();
         }
         if (_type == NONE) {
             return itemData.at(column);
         }
     default:
         return QVariant();
     }
 }

 TaskItem *TaskItem::parent()
 {
     return parentItem;
 }

 int TaskItem::row() const
 {
     if (parentItem)
         return parentItem->childItems.indexOf(const_cast<TaskItem*>(this));

     return 0;
 }

 Task* TaskItem::task() {
     return _task;
 }

 Project* TaskItem::project() {
     return _project;
 }
