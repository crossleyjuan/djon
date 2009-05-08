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


void TaskGrid::drawBackground() {
    QPainter paint (this);
    paint.setPen(Qt::white);
    paint.setBrush(QColor(Qt::white));
    int margin = 1;
    paint.drawRect(margin, margin, this->width()-margin, this->height()-margin);

}

void TaskGrid::drawTask(Task* task, int x, int y) {
    QPainter paint (this);
    paint.setFont(QFont("times", 10, QFont::Bold));
    paint.setBrush(QColor(Qt::red));
    paint.drawText(x, y, QString(task->name.c_str()));
}

void TaskGrid::paintEvent(QPaintEvent*) {
    drawBackground();

    int x = 0;
    int y = 0;
    for (std::vector<Task*>::iterator it = m_project->tasks.begin(); it != m_project->tasks.end(); it++) {
        Task* task = *it;
        drawTask(task, x, y);
        y += 20;
    }
}

TaskGrid::TaskGrid(Project* project, QWidget* parent) : QWidget(parent)
{
    m_project = project;
    std::vector<Task*> tasks = project->tasks;

}

void TaskGrid::onDobleClick(TaskElement* element) {
    TaskDialog* dialog = new TaskDialog(element->getTask(), this);
    dialog->connect(dialog, SIGNAL(accepted()), this, SLOT(onTaskChanged()));
    dialog->exec();
}

void TaskGrid::onTaskChanged() {
    qDebug("test");
}
