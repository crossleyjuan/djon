#include "taskgrid.h"
#include <vector>
#include "taskelement.h"
#include <QVBoxLayout>
#include <sstream>
#include "taskdialog.h"
#include <QLabel>
#include <QGridLayout>

TaskGrid::TaskGrid(Project* project, QWidget* parent) : QWidget(parent)
{
    m_project = project;
    m_size = 0;

//    QGridLayout* grid = new QGridLayout();
    QVBoxLayout* box = new QVBoxLayout();
    box->setDirection(QBoxLayout::TopToBottom);

    setLayout(box);

    updateGrid();
}

void TaskGrid::clearCurrent() {
//    QGridLayout* box = (QGridLayout*)layout();
    QVBoxLayout* box = (QVBoxLayout*)layout();

    QLayoutItem* item;
    while ((item = box->takeAt(0)) != 0) {
        box->removeWidget(item->widget());
        delete item->widget();
/*        delete item;
        */
    }

    /*
    QLayoutItem* item;
    while ((item = lay->takeAt(0)) != 0) {
        delete item;
    }

    for (int x = 0; x < m_size; x++) {
        TaskElement* element = currentElements[x];
        lay->removeWidget(element);
    }
    if (m_size > 0) {
        free(currentElements);
    }
    */
    m_size = 0;
}

void TaskGrid::updateGrid() {
    clearCurrent();

    QVBoxLayout* lay = (QVBoxLayout*)layout();

    //box->addLayout(lay);

    std::vector<Task*> tasks = m_project->tasks;

    currentElements = (TaskElement**)malloc(sizeof(TaskElement) * tasks.size());
    m_size = tasks.size();
    int x = 0;
    for (std::vector<Task*>::iterator it = tasks.begin(); it != tasks.end(); it++) {
        Task* task = *it;
        //QLabel* element = new QLabel(QString(task->shortDescription.c_str()));
        TaskElement* element = new TaskElement(task, this);
        currentElements[++x] = element;
        lay->addWidget(element);
        connect(element, SIGNAL(clicked(TaskElement*)), this, SLOT(onDobleClick(TaskElement*)));
    }
    lay->activate();
}

void TaskGrid::onDobleClick(TaskElement* element) {
    TaskDialog* dialog = new TaskDialog(element->getTask(), this);
    connect(dialog, SIGNAL(taskChanged(Task*)), this, SLOT(onTaskChanged(Task*)));
    dialog->exec();
}

void TaskGrid::updateTask(Task* task) {
    updateGrid();
}

void TaskGrid::onTaskChanged(Task* task) {
    updateTask(task);
}
