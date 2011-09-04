#ifndef ABSTRACTVIEWER_H
#define ABSTRACTVIEWER_H

#include <QModelIndex>
#include <QScrollBar>

class TaskModel;

class AbstractViewer: public QWidget
{
public:
    explicit AbstractViewer(QWidget* parent = 0);
    virtual ~AbstractViewer();
    virtual void setModel(TaskModel* taskModel) = 0;
    virtual QScrollBar* horizontalScrollBar() = 0;
    virtual QScrollBar* verticalScrollBar() = 0;

    virtual void collapse(const QModelIndex& index) = 0;
    virtual void expand(const QModelIndex& index) = 0;
    virtual void scrollToday() = 0;

public slots:
    virtual void refresh() = 0;

};

#endif // ABSTRACTVIEWER_H
