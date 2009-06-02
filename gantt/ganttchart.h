#ifndef GANTTCHART_H
#define GANTTCHART_H

#include <QtGui/QWidget>
#include <vector>
#include "gantttask.h"
#include <QPaintEvent>
#include <QDate>

namespace Ui {
    class GanttChart;
}

enum SCALE {
    DAY, WEEK, HALF_MONTH, MONTH
};

class GanttChart : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(GanttChart)
public:
    explicit GanttChart(QWidget *parent = 0);
    virtual ~GanttChart();
    void addTask(GanttTask* task);
    void setTaskHeight(int height);
    void setHeaderHeight(int height);
    void clear();

protected:
    virtual void changeEvent(QEvent *e);
    void paintEvent(QPaintEvent* evt);

private:
    Ui::GanttChart *m_ui;
    std::vector<GanttTask*> m_tasks;
    void drawBackground();
    void drawTasks();
    void calcZoom();
    bool hasTasks();

// properties
    int m_taskHeight;
    int m_headerHeight;

// calculated values
    QDate* m_startDate;
    QDate* m_endDate;
    SCALE m_scale;
    int m_totalDays;

// CONST
    static const int HEADER_HEIGHT = 30;
    static const int NUM_COLS = 20;
};

#endif // GANTTCHART_H
