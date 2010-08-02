#include "tasklogmodel.h"

#include "data.h"

TaskLogModel::TaskLogModel(Task* task)
{
    _task = task;
    _headerData << "Description" << "Start Date" << "End Date";
//    QAbstractItemModel::setHeaderData(0, Qt::Horizontal, QObject::tr("Description"));
//    QAbstractItemModel::setHeaderData(1, Qt::Horizontal, QObject::tr("Start Date"));
//    QAbstractItemModel::setHeaderData(2, Qt::Horizontal, QObject::tr("End Date"));
}

int TaskLogModel::rowCount(const QModelIndex &parent) const {
    vector<TaskLog*>* logs = _task->logs();
    return logs->size() + 1; // Always creates a new log row
}

int TaskLogModel::columnCount(const QModelIndex &parent) const {
    return 3;
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
        }
    }
    return QString();
}

bool TaskLogModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    qDebug("setdata");
    TaskLog* log = (TaskLog*)index.internalPointer();

    bool newTaskLog = false;
    if (log == NULL) {
        log = new TaskLog();
        log->id = uuid();
        QDateTime dte(QDateTime::currentDateTime());
        log->start = new DateTime(dte.date().year(), dte.date().month(), dte.date().day(), dte.time().hour(), dte.time().minute(), dte.time().second());
        log->end = new DateTime(dte.date().year(), dte.date().month(), dte.date().day(), dte.time().hour(), dte.time().minute(), dte.time().second());
        newTaskLog = true;
        _task->addLog(log);
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
}

QModelIndex TaskLogModel::index(int row, int column, const QModelIndex &parent) const {
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    vector<TaskLog*>* logs = _task->logs();
    if (row < logs->size()) {
        TaskLog* log = logs->at(row);
        return createIndex(row, column, log);
    } else {
        return createIndex(row, column, NULL);
    }
}

Qt::ItemFlags TaskLogModel::flags(const QModelIndex &index) const {
    return QAbstractItemModel::flags(index) | Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}

QVariant TaskLogModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return _headerData.at(section);

    return QVariant();
}
