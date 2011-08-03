#ifndef TASKITEMDELEGATOR_H
#define TASKITEMDELEGATOR_H

#include <QItemDelegate>
#include <QComboBox>
#include "datamanager.h"

class TaskItemDelegator : public QItemDelegate
{
public:
    explicit TaskItemDelegator(QList<CoreTask> lst, QObject *parent = 0);
    // editing
    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const;

signals:

public slots:

private:
    QList<CoreTask> lstData;
};

#endif // TASKITEMDELEGATOR_H
