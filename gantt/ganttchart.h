#ifndef GANTTCHART_H
#define GANTTCHART_H

#include <QtGui/QWidget>
#include <vector>
#include "task.h"
#include <QPaintEvent>

namespace Ui {
    class GanttChart;
}

class GanttChart : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(GanttChart)
public:
    explicit GanttChart(QWidget *parent = 0);
    virtual ~GanttChart();
    void addTask(Task* task);

protected:
    virtual void changeEvent(QEvent *e);
    void paintEvent(QPaintEvent*);

private:
    Ui::GanttChart *m_ui;
    std::vector<Task*> m_tasks;
};

#endif // GANTTCHART_H
