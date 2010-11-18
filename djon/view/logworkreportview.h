#ifndef LOGWORKREPORTVIEW_H
#define LOGWORKREPORTVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>

class TaskModel;

class LogWorkReportView: public QGraphicsView
{
public:
    LogWorkReportView(QWidget *parent = 0);
    LogWorkReportView(QGraphicsScene *scene, QWidget *parent = 0);
    ~LogWorkReportView();
    void setTaskModel(TaskModel* model);

private:
    TaskModel* _taskModel;
};

#endif // LOGWORKREPORTVIEW_H
