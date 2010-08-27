#ifndef GANTTSCENE_H
#define GANTTSCENE_H

#include <QAbstractItemModel>
#include <QGraphicsScene>
#include "util.h"
#include <vector>

class Task;
class TaskModel;

class GanttScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GanttScene(QObject *parent = 0);
    void setModel(TaskModel* model);
    QSize viewSizeHint();
    DateTime startDate();
    DateTime endDate();
    int dayWidth();
    int totalDays();

signals:

public slots:
    void collapse(const QModelIndex& index);
    void expand(const QModelIndex& index);
    void refresh();

protected:
    QGraphicsItem* getTaskItem(const QModelIndex& index);
    QGraphicsItem* getGroupItem(const QModelIndex& index);
    void createBackground();
    void drawIndex(const QModelIndex& index);
    QSize sizeHint(const QModelIndex& index);

private:
    void setupScene();
    void calcZoom();
    void initialize();
    bool isCollapsed(const QModelIndex& index);

    DateTime _startDate;
    DateTime _endDate;
    TaskModel* _model;
    int _viewSizeHeight;
    int _viewSizeWidth;
    int _currentX;
    int _currentY;
    int _dayWidth;
    int _totalDays;
    std::vector<QModelIndex> _collapsedIndexes;
};

#endif // GANTTSCENE_H
