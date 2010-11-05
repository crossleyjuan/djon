#ifndef TASKCLOSEFILTERMODEL_H
#define TASKCLOSEFILTERMODEL_H

#include <QSortFilterProxyModel>

class TaskCloseFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit TaskCloseFilterModel(QObject *parent = 0);

signals:

public slots:

protected:
    virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
};

#endif // TASKCLOSEFILTERMODEL_H
