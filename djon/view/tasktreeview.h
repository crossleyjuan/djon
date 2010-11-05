#ifndef TASKTREEVIEW_H
#define TASKTREEVIEW_H

#include <QTreeView>

class TaskTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit TaskTreeView(QWidget *parent = 0);

signals:

public slots:

protected:
    virtual void drawBranches(QPainter *painter,
                              const QRect &rect,
                              const QModelIndex &index) const;
    virtual void drawRow(QPainter *painter,
                         const QStyleOptionViewItem &options,
                         const QModelIndex &index) const;
};

#endif // TASKTREEVIEW_H
