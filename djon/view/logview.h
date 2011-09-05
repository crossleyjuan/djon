#ifndef LOGVIEW_H
#define LOGVIEW_H

#include "logscene.h"
#include "AbstractViewer.h"
#include <QWidget>
#include <QtGui>

class LogView : public AbstractViewer
{
Q_OBJECT
public:
    explicit LogView(QWidget *parent = 0);
    void setModel(TaskModel* taskModel);
    QScrollBar* horizontalScrollBar();
    QScrollBar* verticalScrollBar();

signals:

public slots:
    void collapse(const QModelIndex& index);
    void expand(const QModelIndex& index);
    void scrollToday();
    void refresh();

protected:
    virtual void resizeEvent(QResizeEvent *);

private:
    void createHeader();
    QSize headerSizeHint();

    QGraphicsView _headerView;
    QGraphicsView _hourView;
    QWidget _separator;
    QGraphicsScene* _hourScene;
    QGraphicsView _logView;
    TaskModel* _model;
    LogScene* _logScene;
    QGraphicsScene* _headerScene;
    int _todayPos;
};

#endif // LOGVIEW_H
