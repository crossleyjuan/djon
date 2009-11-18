#include "logitemmodel.h"
#include "logtime.h"

LogItemModel::LogItemModel(Project* project, Task* task)
{
    m_task = task;
    m_logTimers = readLogTimes(project, task);
}


LogItemModel::~LogItemModel () {
}

int LogItemModel::rowCount(const QModelIndex &parent) const {
    return m_logTimers.size();
}

int LogItemModel::columnCount ( const QModelIndex &parent ) const {
    return 2;
}

QVariant LogItemModel::data ( const QModelIndex &index, int role) const{
    LogTime* logTime = m_logTimers[index.row()];
    QString timeFormat("hh:mm");
    switch (index.column()) {
        case 0:
            return QVariant(logTime->initTime()->toString(Qt::DefaultLocaleLongDate));
        case 1:
            return QVariant(logTime->time()->toString(timeFormat));
    }
    return NULL;
}

