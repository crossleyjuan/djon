#ifndef GANTTVIEW_H
#define GANTTVIEW_H

#include "ganttscene.h"
#include "AbstractViewer.h"
#include <QWidget>
#include <QtGui>

class GanttView : public AbstractViewer
{
Q_OBJECT
public:
    explicit GanttView(QWidget *parent = 0);
    void setModel(TaskModel* taskModel);
    QScrollBar* horizontalScrollBar();
    QScrollBar* verticalScrollBar();

signals:

public slots:
    void collapse(const QModelIndex& index);
    void expand(const QModelIndex& index);
    void scrollToday();
    virtual void refresh();

private:
    void createHeader();
    QSize headerSizeHint();

    QGraphicsView _headerView;
    QGraphicsView _ganttView;
    TaskModel* _model;
    GanttScene* _ganttScene;
    QGraphicsScene* _headerScene;
    int _todayPos;
};

#endif // GANTTVIEW_H
