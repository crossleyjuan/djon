#ifndef GANTTVIEW_H
#define GANTTVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <vector>

class Project;

class GanttView : public QGraphicsView
{
Q_OBJECT
public:
    explicit GanttView(QWidget *parent = 0);
    void setProjects(const std::vector<Project>* projects);

signals:

public slots:

private:
    void setupItems();

    QGraphicsScene* _scene;
    const std::vector<Project>* _projects;
};

#endif // GANTTVIEW_H
