#ifndef TASKGRID_H
#define TASKGRID_H

#include <QWidget>
#include "project.h"

class TaskGrid : public QWidget
{
private:
    Project* m_project;
public:
    TaskGrid(Project* project, QWidget* parent = 0);
};

#endif // TASKGRID_H
