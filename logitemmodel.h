#ifndef LOGITEMMODEL_H
#define LOGITEMMODEL_H

#include <QAbstractTableModel>
#include <vector>
#include "project.h"
#include "logtime.h"

class LogItemModel : public QAbstractTableModel
{
public:
    LogItemModel(Project* project, Task* task);
    ~LogItemModel ();
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

private:
    Project* m_project;
    Task* m_task;
    std::vector<LogTime*> m_logTimers;

};

#endif // LOGITEMMODEL_H
