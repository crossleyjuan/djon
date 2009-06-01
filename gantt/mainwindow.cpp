#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ganttchart.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);
    GanttChart* chart = new GanttChart();

    GanttTask* task = new GanttTask();
    task->setName("test1");
    task->setStartDate(new QDate(2009, 3, 1));
    task->setEndDate(new QDate(2009, 3, 3));
    chart->addTask(task);

    task = new GanttTask();
    task->setName("test2");
    task->setStartDate(new QDate(2009, 3, 3));
    task->setEndDate(new QDate(2009, 3, 5));
    chart->addTask(task);

    task = new GanttTask();
    task->setName("test3");
    task->setStartDate(new QDate(2009, 3, 7));
    task->setEndDate(new QDate(2009, 3, 8));
    chart->addTask(task);

    chart->setTaskHeight(40);
    m_ui->centralwidget->layout()->addWidget(chart);
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
