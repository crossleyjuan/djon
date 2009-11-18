#ifndef TASKMODEL_H
#define TASKMODEL_H

#include <QAbstractItemModel>
#include <vector>

#include "project.h"

class TaskModel : public QAbstractItemModel
    {
        Q_OBJECT

    public:
        TaskModel(const vector<Task*>* rootItems, QObject *parent = 0);
        ~TaskModel();

        QVariant data(const QModelIndex &index, int role) const;
        Qt::ItemFlags flags(const QModelIndex &index) const;
        QVariant headerData(int section, Qt::Orientation orientation,
                            int role = Qt::DisplayRole) const;
        QModelIndex index(int row, int column,
                          const QModelIndex &parent = QModelIndex()) const;
        QModelIndex parent(const QModelIndex &index) const;
        int rowCount(const QModelIndex &parent = QModelIndex()) const;
        int columnCount(const QModelIndex &parent = QModelIndex()) const;

    private:
        void setupModelData(const vector<Task>* rootItems);

        const vector<Task*>* m_rootItems;
    };


#endif // TASKMODEL_H
