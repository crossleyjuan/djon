#include "taskitem.h"
#include <QStringList>

 TaskItem::TaskItem(Project* project, Task* task, TaskItem *parent) {
     parentItem = parent;
     _task = task;
    _project = project;
    _type = TASK;
 }

 TaskItem::TaskItem(std::vector<Project*> projects, TaskItem *parent) {
     parentItem = parent;
     _task = NULL;
    _project = NULL;
    _allprojects = projects;
    _type = SUMMARY;
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
     return 4;
 }

 QVariant TaskItem::data(int column, int role) const
 {
     if (column == 1) {
         if (role == Qt::DisplayRole) {
             return QVariant();
         }
     } else {
         if (role != Qt::DisplayRole) {
             return QVariant();
         }
     }
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
         if (_type == SUMMARY) {
             return "Projects";
         }
     case 1:
         // Closed
         switch (_type) {
         case PROJECT:
         case NONE:
         case SUMMARY:
             return QVariant();
         case TASK:
             return QVariant(_task->isClosed() ? Qt::Checked : Qt::Unchecked);
             return false;
         }
     case 2:
         if (_type == PROJECT) {
             return QString(_project->totalTime()->toChar());
         }
         if (_type == TASK) {
             return QString(_task->totalTime()->toChar());
         }
         if (_type == NONE) {
             return itemData.at(column);
         }
         if (_type == SUMMARY) {
             DTime totalTime;
             for (std::vector<Project*>::const_iterator iter = _allprojects.begin(); iter != _allprojects.end(); iter++) {
                 Project *prj = *iter;
                 totalTime = totalTime + *prj->totalTime();
             }
             return QString(totalTime.toChar());
         }
     case 3:
         if (_type == PROJECT) {
             return QString(_project->totalTimeCurrentWeek()->toChar());
         }
         if (_type == TASK) {
             return QString(_task->totalTimeCurrentWeek()->toChar());
         }
         if (_type == NONE) {
             return itemData.at(column);
         }
         if (_type == SUMMARY) {
             DTime totalTime;
             for (vector<Project*>::const_iterator iter = _allprojects.begin(); iter != _allprojects.end(); iter++) {
                 Project* prj = *iter;
                 totalTime = totalTime + *prj->totalTimeCurrentWeek();
             }
             return QString(totalTime.toChar());
         }
     case 4:
         if (_type == PROJECT) {
             return QString(_project->totalTimeCurrentDay()->toChar());
         }
         if (_type == TASK) {
             return QString(_task->totalTimeCurrentDay()->toChar());
         }
         if (_type == NONE) {
             return itemData.at(column);
         }
         if (_type == SUMMARY) {
             DTime totalTime;
             for (vector<Project*>::const_iterator iter = _allprojects.begin(); iter != _allprojects.end(); iter++) {
                 Project* prj = *iter;
                 totalTime = totalTime + *prj->totalTimeCurrentDay();
             }
             return QString(totalTime.toChar());
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

 void TaskItem::clear() {
     childItems.clear();
 }
