#include "taskgrid.h"
#include <vector>
#include "taskelement.h"
#include <QVBoxLayout>
#include <sstream>

TaskGrid::TaskGrid(Project* project, QWidget* parent) : QWidget(parent)
{
    m_project = project;
    std::vector<Task*> tasks = project->tasks;

    QVBoxLayout* layout = new QVBoxLayout();
    setLayout(layout);

    for (std::vector<Task*>::iterator it = tasks.begin(); it != tasks.end(); it++) {
        Task* task = *it;
        TaskElement* element = new TaskElement(task, this);
        layout->addWidget(element);
        connect(element, SIGNAL(clicked(TaskElement*)), this, SLOT(onDobleClick(TaskElement*)));
    }
}



void TaskGrid::onDobleClick(TaskElement* element) {
    std::stringstream ss;
    ss << "Testing:" << element->getTask()->name;
    qDebug(ss.str().c_str());
}
