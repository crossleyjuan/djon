#include "taskgrid.h"
#include <vector>
#include "taskelement.h"
#include <QVBoxLayout>
#include <sstream>
#include "taskdialog.h"
#include <QLabel>
#include <QGridLayout>
#include "flowlayout.h"
#include "ui_taskgrid.h"
#include "utils.h"

TaskGrid::TaskGrid(Project* project, QWidget* parent) :
        QWidget(parent),
        m_ui(new Ui::TaskGrid)
{
    m_ui->setupUi(this);
    m_project = project;
    m_size = 0;

    FlowLayout* lay = new FlowLayout(5);
//    QVBoxLayout* box = new QVBoxLayout();
//    box->setDirection(QBoxLayout::TopToBottom);

    m_ui->groupBox->setLayout(lay);
    int headerHeight = m_ui->groupBox->height() - m_ui->groupBox->contentsRect().height();
    m_chart = new GanttChart();
    m_chart->setHeaderHeight(headerHeight);
    m_ui->horizontalLayout->addWidget(m_chart);
    updateGrid();
}

void TaskGrid::clearCurrent() {
    FlowLayout* lay = (FlowLayout*)m_ui->groupBox->layout();

    QLayoutItem* item;
    while ((item = lay->takeAt(0)) != 0) {
        lay->removeWidget(item->widget());
        delete item->widget();
    }

    m_size = 0;
    m_chart->clear();
}

void TaskGrid::updateGrid() {
    std::vector<Task*> tasks = readTasks(m_project);

    clearCurrent();

    FlowLayout* lay = (FlowLayout*)m_ui->groupBox->layout();

    currentElements = (TaskElement**)malloc(sizeof(TaskElement) * tasks.size());
    m_size = tasks.size();
    int x = 0;
    for (std::vector<Task*>::iterator it = tasks.begin(); it != tasks.end(); it++) {
        Task* task = *it;
        TaskElement* element = new TaskElement(task, this);
        m_chart->setTaskHeight(element->height());
        currentElements[++x] = element;

        lay->addWidget(element);
        connect(element, SIGNAL(taskFocus(Task*)), this, SLOT(taskFocus(Task*)));

        // Create the chart elements
        GanttTask* gTask = new GanttTask();
        gTask->setName(task->shortDescription);
        QDate startDate = toDateTime(task->startDate)->date();
        gTask->setStartDate(new QDate(startDate.year(), startDate.month(), startDate.day()));
        QDate endDate = toDateTime(task->endDate)->date();
        gTask->setEndDate(new QDate(endDate.year(), endDate.month(), endDate.day()));

        m_chart->addTask(gTask);
    }
    lay->activate();
    m_chart->update();
}

void TaskGrid::updateTask(Task* task) {
    updateGrid();
}

void TaskGrid::taskFocus(Task* task) {
    m_selectedTask = task;
}

Task* TaskGrid::currentTask() {
    return m_selectedTask;
}
