#ifndef LOGWORKREPORTSCENCE_H
#define LOGWORKREPORTSCENCE_H

#include <QGraphicsScene>

class TaskModel;

class LogWorkReportScence : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit LogWorkReportScence(QObject *parent = 0);
    ~LogWorkReportScence();

    void setModel(TaskModel* model);

signals:

public slots:

private:
    void setupScene();

    TaskModel* _model;
};

#endif // LOGWORKREPORTSCENCE_H
