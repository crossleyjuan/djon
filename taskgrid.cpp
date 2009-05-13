#include "taskgrid.h"
#include <vector>
#include "taskelement.h"
#include <QVBoxLayout>
#include <sstream>
#include "taskdialog.h"

TaskGrid::TaskGrid(Project* project, QWidget* parent) : QWidget(parent)
{
    m_project = project;
    QVBoxLayout* layout = new QVBoxLayout();
    setLayout(layout);

    updateGrid();
}

void TaskGrid::updateGrid() {
    QVBoxLayout* layout = (QVBoxLayout*)layout();

    std::vector<Task*> tasks = m_project->tasks;

    for (std::vector<Task*>::iterator it = tasks.begin(); it != tasks.end(); it++) {
        Task* task = *it;
        TaskElement* element = new TaskElement(task, this);
        layout->addWidget(element);
        connect(element, SIGNAL(clicked(TaskElement*)), this, SLOT(onDobleClick(TaskElement*)));
    }
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
