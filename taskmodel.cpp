#include "taskmodel.h"
#include <QtGui>

TaskModel::TaskModel(const vector<Task*>* rootItems, QObject *parent)
    : QAbstractItemModel(parent)
{
    m_rootItems = rootItems;
    //setupModelData(rootItems);
}

TaskModel::~TaskModel()
{
    delete m_rootItems;
}

int TaskModel::columnCount(const QModelIndex &parent) const
{
    return 3;
/*
    if (parent.isValid())
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
*/
}

QVariant TaskModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    Task* task = static_cast<Task*>(index.internalPointer());

    return QString(task->shortDescription.c_str());
}

Qt::ItemFlags TaskModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant TaskModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (section == 1) {
        return "Task Name";
    } else {
        return "Duration";
    }
    /*
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
    */
}

QModelIndex TaskModel::index(int row, int column, const QModelIndex &parent)
        const
{
    Task* task = (*m_rootItems)[row];
    if (task)
        return createIndex(row, column, task);
    else
        return QModelIndex();
}

QModelIndex TaskModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
/*
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
*/
}

int TaskModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        Task* task = static_cast<Task*>(parent.internalPointer());
        return task->childCount;
    } else {
        return m_rootItems->size();
    }
}

void TaskModel::setupModelData(const vector<Task>* rootItems)
{
}
