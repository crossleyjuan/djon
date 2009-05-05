#ifndef TASKELEMENT_H
#define TASKELEMENT_H

#include <QWidget>
#include "project.h"
#include <qpainter.h>

class TaskElement : public QWidget
{
    Q_OBJECT

protected:
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent *event);

public:
    TaskElement(Task* task, QWidget* parent = 0);
    Task* getTask();

signals:
    void clicked(TaskElement* element);

private:
    Task* m_task;
};

#endif // TASKELEMENT_H
