#include "ganttview.h"

GanttView::GanttView(QWidget *parent) :
    QGraphicsView(parent)
{
    _scene = new QGraphicsScene(this);
    this->_projects = NULL;
}

void GanttView::setProjects(const std::vector<Project> *projects) {
    _projects = projects;
    setupItems();
}

void GanttView::setupItems() {

}
