#ifndef TASKHEADERVIEW_H
#define TASKHEADERVIEW_H

#include <QtGui>

class TaskHeaderView : public QHeaderView
{
Q_OBJECT
public:
    explicit TaskHeaderView(Qt::Orientation orientation, QWidget *parent = 0);

protected:
    void paintSection ( QPainter * painter, const QRect & rect, int logicalIndex ) const;

signals:

public slots:

};

#endif // TASKHEADERVIEW_H
