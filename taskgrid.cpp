#include "taskgrid.h"
#include <vector>
#include "taskelement.h"
#include <QVBoxLayout>
#include <sstream>
#include "taskdialog.h"

void TaskGrid::addTask(Task* task) {
    QLayout* layout = this->layout();
    TaskElement* element = new TaskElement(task, this);
    layout->addWidget(element);
    connect(element, SIGNAL(clicked(TaskElement*)), this, SLOT(onDobleClick(TaskElement*)));
}

TaskGrid::TaskGrid(Project* project, QWidget* parent) : QWidget(parent)
{
    m_project = project;
    std::vector<Task*> tasks = project->tasks;

    QVBoxLayout* layout = new QVBoxLayout();
    setLayout(layout);

    for (std::vector<Task*>::iterator it = tasks.begin(); it != tasks.end(); it++) {
        Task* task = *it;
        addTask(task);
    }
}

void TaskGrid::onDobleClick(TaskElement* element) {
    TaskDialog* dialog = new TaskDialog(element->getTask(), this);
    dialog->connect(dialog, SIGNAL(accepted()), this, SLOT(onTaskChanged()));
    dialog->exec();
}

void TaskGrid::onTaskChanged() {
    qDebug("test");
}
