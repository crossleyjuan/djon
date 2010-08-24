#ifndef GANTTSCENE_H
#define GANTTSCENE_H

#include <QAbstractItemModel>
#include <QGraphicsScene>
#include "util.h"

class Task;
class TaskModel;

class GanttScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GanttScene(QObject *parent = 0);
    void setModel(TaskModel* model);
    QGraphicsItem* getTaskItem(Task* task);

signals:

public slots:

protected:
    void drawBackground(QPainter *painter, const QRectF &rect);

private:
    void setupScene();
    void calcZoom();

    DateTime* _startDate;
    DateTime* _endDate;
    TaskModel* _model;
};

#endif // GANTTSCENE_H
