#ifndef TIMEVIEW_H
#define TIMEVIEW_H

#include "timescene.h"
#include "AbstractViewer.h"
#include <QWidget>
#include <QtGui>

class TimeView : public AbstractViewer
{
Q_OBJECT
public:
    explicit TimeView(QWidget *parent = 0);
    void setModel(TaskModel* taskModel);
    QScrollBar* horizontalScrollBar();
    QScrollBar* verticalScrollBar();

signals:
    void itemHoverEnter(QModelIndex index);
    void itemHoverLeave(QModelIndex index);

public slots:
    void collapse(const QModelIndex& index);
    void expand(const QModelIndex& index);
    void scrollToday();
    void receiveItemHoverEnter(QModelIndex index);
    void receiveItemHoverLeave(QModelIndex index);
    virtual void refresh();

private:
    void createHeader();
    QSize headerSizeHint();

    QGraphicsView _headerView;
    QGraphicsView _timeView;
    TaskModel* _model;
    TimeScene* _timeScene;
    QGraphicsScene* _headerScene;
    int _todayPos;
};

#endif // TIMEVIEW_H
