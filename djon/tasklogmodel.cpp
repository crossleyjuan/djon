#include "tasklogmodel.h"
#include "data.h"
#include <vector>
#include <algorithm>
#include <sstream>

int __sortColumn;
Qt::SortOrder __sortOrder;

TaskLogModel::TaskLogModel(Task* task)
{
    _task = task;
    _logs = _task->logs();

    _headerData << "Description" << "Start Date" << "End Date" << "Time";
}

int TaskLogModel::rowCount(const QModelIndex &parent) const {
    return _logs->size() + 1; // Always creates a new log row
}

int TaskLogModel::columnCount(const QModelIndex &parent) const {
    return 4;
}

QVariant TaskLogModel::data(const QModelIndex &index, int role) const {
    //    if (role != Qt::ItemDataRole) {
    //        return QVariant();
    //    }

    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    TaskLog* log = (TaskLog*)index.internalPointer();
    if (log != NULL) {
        if (index.column() == 0) {
            if (log->logDescription != NULL) {
                return QString(log->logDescription->c_str());
            } else {
                return QString();
            }
        } else if (index.column() == 1) {
            return *log->start->toQDateTime();
        } else if (index.column() == 2) {
            return *log->end->toQDateTime();
        } else if (index.column() == 3) {
            return QString(DTime(*log->end - *log->start).toChar());
        }
    }
    return QString();
}

bool TaskLogModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    qDebug("TaskLogModel::setData");
    TaskLog* log = (TaskLog*)index.internalPointer();

    bool newTaskLog = false;
    if (log == NULL) {
        beginResetModel();
        log = createTaskLog(_task);
        QDateTime dte(QDateTime::currentDateTime());
        log->start = new DateTime(dte.date().year(), dte.date().month(), dte.date().day(), dte.time().hour(), dte.time().minute(), dte.time().second());
        log->end = new DateTime(dte.date().year(), dte.date().month(), dte.date().day(), dte.time().hour(), dte.time().minute(), dte.time().second());
        newTaskLog = true;
        endResetModel();
        emit dataChanged(QModelIndex(), index);
    }
    if (index.column() == 0) {
        QString descr = value.toString();
        log->logDescription = new string(descr.toStdString());
    }
    if (index.column() == 1) {
        QDateTime time = value.toDateTime();
        log->start = new DateTime(time);
    }
    if (index.column() == 2) {
        QDateTime time = value.toDateTime();
        log->end = new DateTime(time);
    }
    if (newTaskLog) {
        createTaskLog(_task, log);
    } else {
        updateTaskLog(_task, log);
    }
    return true;
}

QModelIndex TaskLogModel::index(int row, int column, const QModelIndex &parent) const {
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    if (row < (int)_logs->size()) {
        TaskLog* log = _logs->at(row);
        return createIndex(row, column, log);
    } else {
        return createIndex(row, column, (void*)NULL);
    }
}

Qt::ItemFlags TaskLogModel::flags(const QModelIndex &index) const {
    if (index.column() != 3) {
        return QAbstractItemModel::flags(index) | Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
    } else {
        return QAbstractItemModel::flags(index) | Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    }
}

QVariant TaskLogModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return _headerData.at(section);

    return QVariant();
}

bool compare(TaskLog* log1, TaskLog* log2) {
    bool result = false;
    if ((log1 == NULL) && (log2 != NULL)) {
        result = false;
    } else if ((log1 != NULL) && (log2 == NULL)) {
        result = true;
    } else if ((log1 == NULL) && (log2 == NULL)) {
        result = (log1->id->compare(*log2->id) < 0);
    } else {
        switch (__sortColumn) {
            case 0: // Description
                if ((log1->logDescription == NULL) && (log2->logDescription != NULL)) {
                    result = false;
                } else if ((log1->logDescription != NULL) && (log2->logDescription == NULL)) {
                    result = true;
                } else if ((log1->logDescription == NULL) && (log2->logDescription == NULL)) {
                    result = (log1->id->compare(*log2->id) < 0);
                } else {
                    int com = log1->logDescription->compare(*log2->logDescription);
                    if (com == 0) {
                        result = (log1->id->compare(*log2->id) < 0);
                    } else {
                        result = (com < 0);
                    }
                }
                break;
            case 1: // Start
                result = (*log1->start < *log2->start);
                break;
            case 2: // End
                result = (*log1->end < *log2->end);
                break;
            default:
                throw;
        }
    }

    if (__sortOrder == Qt::DescendingOrder) {
        result = !result;
    }
    return result;
}

void TaskLogModel::sort(int column, Qt::SortOrder order) {
    __sortColumn = column;
    __sortOrder = order;
    std::sort(_logs->begin(), _logs->end(), compare);
    reset();
}
