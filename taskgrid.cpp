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
    m_activeTaskElement = NULL;
    m_timerEnabled = false;

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

TaskGrid::~TaskGrid() {
    if (m_activeTaskElement != NULL) {
        m_activeTaskElement->stopTimeRecord();
    }
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
    m_activeTaskElement = NULL;
}

void TaskGrid::updateGrid() {
    std::vector<Task*> tasks = readTasks(m_project);

    std::string currentId = "";

    if (m_activeTaskElement != NULL) {
        currentId = m_activeTaskElement->task()->id;
    }

    clearCurrent();

    FlowLayout* lay = (FlowLayout*)m_ui->groupBox->layout();

    currentElements = (TaskElement**)malloc(sizeof(TaskElement) * tasks.size());
    m_size = 0;
    int x = 0;
    for (std::vector<Task*>::iterator it = tasks.begin(); it != tasks.end(); it++) {
        Task* task = *it;

        Template* temp = readTemplate(task->templateName);
        if ((temp != NULL) && (task->status.compare(temp->closedStatus()) == 0)) {
            continue;
        }
        TaskElement* element = new TaskElement(m_project, task, this);
        if (task->id.compare(currentId) == 0) {
            m_activeTaskElement = element;
        }
        m_chart->setTaskHeight(element->height());
        currentElements[x] = element;

        lay->addWidget(element);
        connect(element, SIGNAL(taskActive(TaskElement*)), this, SLOT(taskActive(TaskElement*)));
        connect(element, SIGNAL(taskSelected(TaskElement*)), this, SLOT(taskSelected(TaskElement*)));

        // Create the chart elements
        GanttTask* gTask = new GanttTask();
        gTask->setName(task->shortDescription);
        QDate startDate = toDateTime((int)task->startDate)->date();
        gTask->setStartDate(new QDate(startDate.year(), startDate.month(), startDate.day()));
        QDate endDate = toDateTime((int)task->endDate)->date();
        gTask->setEndDate(new QDate(endDate.year(), endDate.month(), endDate.day()));

        m_chart->addTask(gTask);
        if (x == 0) { // First element
            element->selectTask();
        }
        x++;
        m_size++;
    }
    lay->activate();
    m_chart->update();
}

void TaskGrid::updateTask(Task* task) {
    updateGrid();
}

void TaskGrid::taskActive(TaskElement* task) {
    m_activeTaskElement = task;
    for (int x = 0; x < m_size; x++) {
        if (currentElements[x]->task()->id.compare(m_activeTaskElement->task()->id) == 0) {
            currentElements[x]->setActive(true);
            if (m_timerEnabled) {
                currentElements[x]->startTimeRecord();
            }
        } else {
            currentElements[x]->setActive(false);
            if (m_timerEnabled && currentElements[x]->isTimeRunning()) {
                currentElements[x]->stopTimeRecord();
            }
        }
    }
}

void TaskGrid::taskSelected(TaskElement* task) {
    m_selectedTaskElement = task;
}

Task* TaskGrid::selectedTask() {
    return m_selectedTaskElement->task();
}

TaskElement* TaskGrid::activeTaskElement() {
    return m_activeTaskElement;
}

void TaskGrid::setTimerEnabled(bool timerEnabled) {
    m_timerEnabled = timerEnabled;

    if (timerEnabled) {
        m_activeTaskElement->startTimeRecord();
    } else {
        for (int x = 0; x < m_size; x++) {
            currentElements[x]->stopTimeRecord();
        }
    }
}
