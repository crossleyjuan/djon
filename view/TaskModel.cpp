/* 
 * File:   TaskModel.cpp
 * Author: cross
 * 
 * Created on July 9, 2010, 9:06 AM
 */

#include "TaskModel.h"
#include <QtGui>

TaskModel::TaskModel(const vector<Project*> projects, QObject *parent)
     : QAbstractItemModel(parent)
 {
     QList<QVariant> rootData;
     _projects = projects;
     rootData << "Description" << "Total Time" << "Gantt";
     rootItem = new TaskItem(rootData);
     setupModelData(rootItem);
 }

 TaskModel::~TaskModel()
 {
     delete rootItem;
 }

 int TaskModel::columnCount(const QModelIndex &parent) const
 {
     if (parent.isValid())
         return static_cast<TaskItem*>(parent.internalPointer())->columnCount();
     else
         return rootItem->columnCount();
 }

 QVariant TaskModel::data(const QModelIndex &index, int role) const
 {
     if (!index.isValid())
         return QVariant();

     if (role != Qt::DisplayRole)
         return QVariant();

     TaskItem *item = static_cast<TaskItem*>(index.internalPointer());

     return item->data(index.column());
 }

 Qt::ItemFlags TaskModel::flags(const QModelIndex &index) const
 {
     return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
 }

 QVariant TaskModel::headerData(int section, Qt::Orientation orientation,
                                int role) const
 {
     if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
         return rootItem->data(section);

     return QVariant();
 }

 QModelIndex TaskModel::index(int row, int column, const QModelIndex &parent)
             const
 {
     if (!hasIndex(row, column, parent))
         return QModelIndex();

     TaskItem *parentItem;

     if (!parent.isValid())
         parentItem = rootItem;
     else
         parentItem = static_cast<TaskItem*>(parent.internalPointer());

     TaskItem *childItem = parentItem->child(row);
     if (childItem)
         return createIndex(row, column, childItem);
     else
         return QModelIndex();
 }

 QModelIndex TaskModel::parent(const QModelIndex &index) const
 {
     if (!index.isValid())
         return QModelIndex();

     TaskItem *childItem = static_cast<TaskItem*>(index.internalPointer());
     TaskItem *parentItem = childItem->parent();

     if (parentItem == rootItem)
         return QModelIndex();

     return createIndex(parentItem->row(), 0, parentItem);
 }

 int TaskModel::rowCount(const QModelIndex &parent) const
 {
     TaskItem *parentItem;
     if (parent.column() > 0)
         return 0;

     if (!parent.isValid())
         parentItem = rootItem;
     else
         parentItem = static_cast<TaskItem*>(parent.internalPointer());

     return parentItem->childCount();
 }

 void TaskModel::setupModelData(TaskItem *parent)
 {
     QHash<QString, TaskItem*> hash;

     for (vector<Project*>::iterator iter = _projects.begin(); iter != _projects.end(); iter++) {
         int position = 0;
         Project* project = *iter;
         vector<Task*>* tasks = project->tasks();

         TaskItem* projectItem = new TaskItem(project, parent);
         hash[QString(project->name()->c_str())] = projectItem;
         qDebug("Project: %s", project->description()->c_str());
         parent->appendChild(projectItem);

         for (vector<Task*>::iterator iterTask = tasks->begin(); iterTask != tasks->end(); iterTask++) {
             Task* task = *iterTask;
             QString taskId(task->id()->c_str());

             int lastDot = taskId.lastIndexOf(".");
             TaskItem* root;
             if (lastDot > -1) {
                 root = hash[taskId.left(lastDot)];
             } else {
                 root = projectItem;
             }

             // Append a new item to the current parent's list of children.
             TaskItem* item = new TaskItem(project, task, root);
             root->appendChild(item);
             hash[taskId] = item;
         }
     }
 }


 Task* TaskModel::task(const QModelIndex &index) const {
     if (index.isValid()) {
         TaskItem *item = static_cast<TaskItem*>(index.internalPointer());

         return item->task();
     }
}

 Project* TaskModel::project(const QModelIndex &index) const {
     if (index.isValid()) {
         TaskItem *item = static_cast<TaskItem*>(index.internalPointer());

         return item->project();
     }
}

 bool TaskModel::setData(const QModelIndex &index, const QVariant &value, int role) {
     if (value.canConvert<QString>()) {
         QString s = value.toString();
         TaskItem *item = static_cast<TaskItem*>(index.internalPointer());
         if (item->task() != NULL) {
             item->task()->setShortDescription(new string(s.toStdString()));
         }
     }
 }
