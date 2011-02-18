#ifndef TIMESCENE_H
#define TIMESCENE_H

#include <QAbstractItemModel>
#include <QGraphicsScene>
#include "util.h"
#include <vector>

class Task;
class TaskModel;
class TaskLog;

class TimeScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit TimeScene(QObject *parent = 0);
    void setModel(TaskModel* model);
    QSize viewSizeHint();
    DateTime startDate();
    DateTime endDate();
    int dayWidth();
    int totalDays();

signals:
    void itemHoverEnter(QModelIndex index);
    void itemHoverLeave(QModelIndex index);

public slots:
    void collapse(const QModelIndex& index);
    void expand(const QModelIndex& index);
    void refresh();
    void receiveItemHoverEnter(QModelIndex index);
    void receiveItemHoverLeave(QModelIndex index);

protected:
    void drawTimeLog(const QModelIndex& index);
    void createBackground();
    void drawIndex(const QModelIndex& index);
    QSize sizeHint(const QModelIndex& index);

private:
    void setupScene();
    void calcZoom();
    void initialize();
    bool isCollapsed(const QModelIndex& index);
    static bool compareTaskLog(TaskLog* log1, TaskLog* log2);
    void drawTime(DateTime currentDay, DTime time, QModelIndex index, int groupLevel);

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

#endif // TIMESCENE_H
