#ifndef TASKGRID_H
#define TASKGRID_H

#include <QWidget>
#include "project.h"
#include "taskelement.h"

class TaskGrid : public QWidget
{
    Q_OBJECT

public:
    TaskGrid(Project* project, QWidget* parent = 0);

public slots:
    void onDobleClick(TaskElement* element);
    void onTaskChanged();
protected:
    void paintEvent(QPaintEvent*);

private:
    Project* m_project;
    void drawTask(Task* task, int x, int y);
    void addTask(Task* task);
    void drawBackground();
};

#endif // TASKGRID_H
