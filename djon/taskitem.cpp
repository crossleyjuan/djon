// *********************************************************************************************************************
// file:    taskitem.cpp
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

 TaskItem::TaskItem(TaskItem *parent) {
     parentItem = parent;
     _task = NULL;
    _project = NULL;
    _type = BLANK;
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
     if (_type == BLANK) {
         return QVariant();
     }
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
             return _project->totalTime().toQString();
         }
         if (_type == TASK) {
             return _task->totalTime().toQString();
         }
         if (_type == NONE) {
             return itemData.at(column);
         }
         if (_type == SUMMARY) {
             DTime totalTime;
             for (std::vector<Project*>::const_iterator iter = _allprojects.begin(); iter != _allprojects.end(); iter++) {
                 Project *prj = *iter;
                 totalTime = totalTime + prj->totalTime();
             }
             return totalTime.toQString();
         }
     case 3:
         if (_type == PROJECT) {
             return _project->totalTimeCurrentWeek().toQString();
         }
         if (_type == TASK) {
             return _task->totalTimeCurrentWeek().toQString();
         }
         if (_type == NONE) {
             return itemData.at(column);
         }
         if (_type == SUMMARY) {
             DTime totalTime;
             for (vector<Project*>::const_iterator iter = _allprojects.begin(); iter != _allprojects.end(); iter++) {
                 Project* prj = *iter;
                 totalTime = totalTime + prj->totalTimeCurrentWeek();
             }
             return totalTime.toQString();
         }
     case 4:
         if (_type == PROJECT) {
             return _project->totalTimeCurrentDay().toQString();
         }
         if (_type == TASK) {
             return _task->totalTimeCurrentDay().toQString();
         }
         if (_type == NONE) {
             return itemData.at(column);
         }
         if (_type == SUMMARY) {
             DTime totalTime;
             for (vector<Project*>::const_iterator iter = _allprojects.begin(); iter != _allprojects.end(); iter++) {
                 Project* prj = *iter;
                 totalTime = totalTime + prj->totalTimeCurrentDay();
             }
             return totalTime.toQString();
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

 void TaskItem::removeChild(TaskItem *child) {
     childItems.removeOne(child);
 }

 ELEMENT_TYPE TaskItem::type() {
     return _type;
 }
