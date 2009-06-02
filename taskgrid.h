#ifndef TASKGRID_H
#define TASKGRID_H

#include <QWidget>
#include "project.h"
#include "taskelement.h"
#include "gantt/ganttchart.h"

namespace Ui {
    class TaskGrid;
}

class TaskGrid : public QWidget
{
    Q_OBJECT

public:
    TaskGrid(Project* project, QWidget* parent = 0);
    Task* currentTask();

public slots:
    void taskFocus(Task* task);
    void updateGrid();

private:
    Project* m_project;
    void addTask(Task* task);
    void updateTask(Task* task);
    TaskElement** currentElements;
    void clearCurrent();
    int m_size;
    GanttChart* m_chart;
    Ui::TaskGrid *m_ui;
    Task* m_selectedTask;
};

#endif // TASKGRID_H
