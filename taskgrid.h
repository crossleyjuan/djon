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

public slots:
    void onDobleClick(TaskElement* element);
    void onTaskChanged(Task* task);
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
};

#endif // TASKGRID_H
