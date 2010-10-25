/* 
 * File:   TaskModel.cpp
 * Author: cross
 * 
 * Created on July 9, 2010, 9:06 AM
 */

#include "TaskModel.h"
#include <QtGui>

TaskModel::TaskModel(MODELTYPE type, const vector<Project*> projects, QObject *parent)
    : QAbstractItemModel(parent)
{
    _type = type;
    _projects = projects;
    _trackedTask = NULL;

    refreshData();
}

TaskModel::~TaskModel()
{
    delete rootItem;
}

int TaskModel::columnCount(const QModelIndex &parent) const
{
    switch (_type) {
        case ONLY_TASKS:
            return 1;
        case WITH_TIMES:
            return 4;
    default:
            return 0;
    }

//    if (parent.isValid())
//        return static_cast<TaskItem*>(parent.internalPointer())->columnCount();
//    else
//        return rootItem->columnCount();
}

QVariant TaskModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::BackgroundRole) {
        if (_trackedTask != NULL) {
            Task* indexTask = task(index);
            if ((indexTask != NULL) && (*indexTask == *_trackedTask)) {
                return qVariantFromValue(QColor(Qt::yellow));
            }
        }
        return QVariant();
    }

    if (role == Qt::TextAlignmentRole) {
        if (index.column() > 0) {
            return Qt::AlignRight;
        } else {
            return QVariant();
        }
    }
    if (role == Qt::SizeHintRole) {
        return QSize(10, 15);
    }

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

//    if (role == Qt::SizeHintRole)
//        return QSize(50, 20);
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
        Project* project = *iter;
        vector<Task*>* tasks = project->tasks();

        TaskItem* projectItem = new TaskItem(project, parent);
        hash[QString(project->name()->c_str())] = projectItem;
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
    return NULL;
}

Project* TaskModel::project(const QModelIndex &index) const {
    if (index.isValid()) {
        TaskItem *item = static_cast<TaskItem*>(index.internalPointer());

        return item->project();
    }
    return NULL;
}

bool TaskModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (value.canConvert<QString>()) {
        QString s = value.toString();
        TaskItem *item = static_cast<TaskItem*>(index.internalPointer());
        if (item->task() != NULL) {
            item->task()->setShortDescription(new string(s.toStdString()));
        }
    }
    return false;
}

void TaskModel::setTrackedTask(Task *task) {
    _trackedTask = task;
}

Task* TaskModel::trackedTask() const {
    return _trackedTask;
}

void TaskModel::setProjects(const vector<Project*> projects) {
    _projects = projects;
    refreshData();
}

void TaskModel::refreshData() {
    QList<QVariant> rootData;
    if (_type == ONLY_TASKS) {
        rootData << "Description";
    } else {
        rootData << "Description" << "Total Time" << "Week" << "Day";
    }
    rootItem = new TaskItem(rootData);
    TaskItem* summary = new TaskItem(_projects, rootItem);
    rootItem->appendChild(summary);
    setupModelData(summary);
    // resets the views states (invalidate previously sent data)
//    reset();
}

QModelIndex TaskModel::index(Project* prj) {
    QModelIndex summary = index(0, 0, QModelIndex());
    int rows = rowCount(summary);
    for (int row = 0; row < rows; row++) {
        QModelIndex test = index(row, 0, summary);
        Project* testPrj = project(test);
        if ((testPrj != NULL) && (*testPrj == *prj)) {
            return test;
        }
    }
    return QModelIndex();
}

QModelIndex TaskModel::search(QModelIndex root, Task* tsk) {
    for (int row = 0; row < rowCount(root); row++) {
        QModelIndex test = index(row, 0, root);
        Task* testTask = task(test);
        if ((testTask != NULL) && (*testTask == *tsk)) {
            return test;
        }
        if (rowCount(test) > 0) {
            test = search(test, tsk);
            if (test.isValid()) {
                return test;
            }
        }
    }
    return QModelIndex();
}

QModelIndex TaskModel::index(Project* project, Task* task) {
    QModelIndex indexProject = index(project);
    if (task != NULL) {
        if (indexProject.isValid()) {
            return search(indexProject, task);
        } else {
            return QModelIndex();
        }
    } else {
        return indexProject;
    }
}

void TaskModel::timeChanged(Task* task) {
//    QModelIndex taskIndex = index(task->project(), task);

//    QModelIndex timeIndex = index(taskIndex.row(), 1, taskIndex.parent());
//    emit dataChanged(timeIndex, timeIndex);
}
