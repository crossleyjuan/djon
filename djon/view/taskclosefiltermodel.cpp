#include "taskclosefiltermodel.h"
#include "TaskModel.h"
#include "data.h"

TaskCloseFilterModel::TaskCloseFilterModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}


bool TaskCloseFilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const {
    TaskModel* model = (TaskModel*)sourceModel();

    QModelIndex index = model->index(source_row, 0 , source_parent);
    Task* task = model->task(index);
    if (task == NULL) {
        return true;
    } else {
        return task->isClosed();
    }
}
