#ifndef TASKLOGMODEL_H
#define TASKLOGMODEL_H

#include <QtGui>
#include <vector>

class Task;
class TaskLog;

class TaskLogModel : public QAbstractTableModel
{
public:
    TaskLogModel(Task* task);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

private:
    Task* _task;
    QList<QVariant> _headerData;
    std::vector<TaskLog*>* _logs;
};

#endif // TASKLOGMODEL_H
