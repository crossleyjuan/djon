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

private:
    Project* m_project;
    void addTask(Task* task);

};

#endif // TASKGRID_H
