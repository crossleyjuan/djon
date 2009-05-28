#include "taskgrid.h"
#include <vector>
#include "taskelement.h"
#include <QVBoxLayout>
#include <sstream>
#include "taskdialog.h"
#include <QLabel>
#include <QGridLayout>
#include "flowlayout.h"

TaskGrid::TaskGrid(Project* project, QWidget* parent) : QWidget(parent)
{
    m_project = project;
    m_size = 0;

    FlowLayout* lay = new FlowLayout(5);
//    QVBoxLayout* box = new QVBoxLayout();
//    box->setDirection(QBoxLayout::TopToBottom);

    setLayout(lay);

    updateGrid();
}

void TaskGrid::clearCurrent() {
    FlowLayout* lay = (FlowLayout*)layout();

    QLayoutItem* item;
    while ((item = lay->takeAt(0)) != 0) {
        lay->removeWidget(item->widget());
        delete item->widget();
    }

    m_size = 0;
}

void TaskGrid::updateGrid() {
    std::vector<Task*> tasks = readTasks(m_project);

    clearCurrent();

    FlowLayout* lay = (FlowLayout*)layout();

    currentElements = (TaskElement**)malloc(sizeof(TaskElement) * tasks.size());
    m_size = tasks.size();
    int x = 0;
    for (std::vector<Task*>::iterator it = tasks.begin(); it != tasks.end(); it++) {
        Task* task = *it;
        TaskElement* element = new TaskElement(task, this);
        currentElements[++x] = element;
        lay->addWidget(element);
        connect(element, SIGNAL(clicked(TaskElement*)), this, SLOT(onDobleClick(TaskElement*)));
    }
    lay->activate();
}

void TaskGrid::onDobleClick(TaskElement* element) {
    TaskDialog* dialog = new TaskDialog(m_project, element->getTask(), this);
    connect(dialog, SIGNAL(taskChanged(Task*)), this, SLOT(onTaskChanged(Task*)));
    dialog->exec();
}

void TaskGrid::updateTask(Task* task) {
    updateGrid();
}

void TaskGrid::onTaskChanged(Task* task) {
    updateTask(task);
}
