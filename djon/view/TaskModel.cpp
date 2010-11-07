/* 
 * File:   TaskModel.cpp
 * Author: cross
 * 
 * Created on July 9, 2010, 9:06 AM
 */

#include "TaskModel.h"
#include "closedtaskfilter.h"
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
            return 5;
    default:
            return 0;
    }
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


    TaskItem *item = static_cast<TaskItem*>(index.internalPointer());

    return item->data(index.column(), role);
}

Qt::ItemFlags TaskModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
    if (index.column() == 1) {
        flags = flags | Qt::ItemIsUserCheckable;
    }
    return flags;
}

QVariant TaskModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section, role);

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
    parent->clear();
    QHash<QString, TaskItem*> hash;

    Task* lastTask = NULL;
    for (vector<Project*>::iterator iter = _projects.begin(); iter != _projects.end(); iter++) {
        Project* project = *iter;
        vector<Task*>* tasks = project->tasks();

        TaskItem* projectItem = new TaskItem(project, parent);
        hash[QString(project->name()->c_str())] = projectItem;
        parent->appendChild(projectItem);

        for (vector<Task*>::iterator iterTask = tasks->begin(); iterTask != tasks->end(); iterTask++) {
            Task* task = *iterTask;
            if (!acceptFilter(task)) {
                continue;
            }
            QString taskId(task->id()->c_str());

            int lastDot = taskId.lastIndexOf(".");
            TaskItem* root;
            if (lastDot > -1) {
                QString tskId = taskId.left(lastDot);
                root = hash[tskId];
                Task* tsk = project->task(tskId.toStdString());
                if (!acceptFilter(tsk)) {
                    continue;
                }
            } else {
                root = projectItem;
            }

            // Append a new item to the current parent's list of children.
            TaskItem* item = new TaskItem(project, task, root);
            root->appendChild(item);
            hash[taskId] = item;
            lastTask = task;
        }
    }
    if (lastTask != NULL) {
        QModelIndex lastIndex = index(lastTask->project(), lastTask);
        emit this->dataChanged(QModelIndex(), lastIndex);
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
    Task* tsk = task(index);
    if (tsk == NULL) {
        return false;
    }
    bool modified = false;
    switch (index.column()) {
    case 0: // Description
        tsk->setShortDescription(new std::string(value.toString().toStdString()));
        modified = true;
        break;
    case 1:
        tsk->setClosed(value.toBool());
        modified = true;
        break;
    }
    // Saves the change
    if (modified) {
        updateTask(tsk);
        if (!acceptFilter(tsk)) {
            beginRemoveRows(index.parent(), index.row(), index.row());
            TaskItem* item = (TaskItem*)index.internalPointer();
            TaskItem* parentItem = item->parent();
            parentItem->removeChild(item);
            endRemoveRows();
        }
        emit dataChanged(index, index);
    }
    return modified;
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
    beginResetModel();
    QList<QVariant> rootData;
    if (_type == ONLY_TASKS) {
        rootData << "Description";
    } else {
        rootData << "Description" << "" << "Total Time" << "Week" << "Day";
    }
    rootItem = new TaskItem(rootData);
    TaskItem* summary = new TaskItem(_projects, rootItem);
    rootItem->appendChild(summary);
    setupModelData(summary);
    endResetModel();
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
//
//    QModelIndex timeIndex = index(taskIndex.row(), 3, taskIndex.parent());
//    emit dataChanged(timeIndex, timeIndex);
}

void TaskModel::addFilter(TASKFILTER_TYPE filterType) {
    AbstractTaskFilter* filter;
    switch (filterType) {
    case CLOSED_FILTER:
        filter = new ClosedTaskFilter();
        break;
    }

    _filters.push_back(filter);
    refreshData();
}

void TaskModel::removeFilter(TASKFILTER_TYPE filterType) {
    switch (filterType) {
    case CLOSED_FILTER:
        vector<const AbstractTaskFilter*>::iterator iter;
        for (iter = _filters.begin(); iter != _filters.end(); iter++) {
            const AbstractTaskFilter* filter = *iter;
            if (filter->type() == filterType) {
                break;
            }
        }
        if (iter != _filters.end()) {
            _filters.erase(iter);
        }
        break;
    }
    refreshData();
}

bool TaskModel::acceptFilter(Task *task) {
    bool accepted = true;
    for (std::vector<const AbstractTaskFilter*>::iterator iter = _filters.begin(); iter != _filters.end(); iter++) {
        const AbstractTaskFilter* filter = *iter;
        accepted = accepted && filter->acceptTask(task);
        if (!accepted) {
            break;
        }
    }
    return accepted;
}

void TaskModel::removeTask(Task* tsk) {
    QModelIndex idx = index(tsk->project(), tsk);

    beginRemoveRows(idx.parent(), idx.row(), idx.row());

    TaskItem* item = (TaskItem*)idx.parent().internalPointer();
    item->removeChild((TaskItem*)idx.internalPointer());

    endRemoveRows();
}
