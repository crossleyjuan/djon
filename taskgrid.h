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
    ~TaskGrid();
    TaskElement* currentTaskElement();
    void setTimerEnabled(bool timerEnabled);

public slots:
    void taskFocus(TaskElement* task);
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
    TaskElement* m_selectedTaskElement;
    bool m_timerEnabled;
};

#endif // TASKGRID_H
