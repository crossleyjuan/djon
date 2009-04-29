#ifndef TASKELEMENT_H
#define TASKELEMENT_H

#include <QWidget>
#include "project.h"
#include <qpainter.h>

class TaskElement : public QWidget
{
private:
    Task* m_task;
protected:
    void paintEvent(QPaintEvent*);
public:
    TaskElement(Task* task, QWidget* parent = 0);
};

#endif // TASKELEMENT_H
