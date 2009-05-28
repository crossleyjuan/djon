#ifndef TASKELEMENT_H
#define TASKELEMENT_H

#include <QWidget>
#include "project.h"
#include <qpainter.h>
#include <QLineEdit>

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
    void gotFocus(TaskElement* element);

private:
    Task* m_task;
    QLineEdit *m_txtName;
    QLineEdit *m_txtDuration;
};

#endif // TASKELEMENT_H
