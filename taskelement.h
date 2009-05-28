#ifndef TASKELEMENT_H
#define TASKELEMENT_H

#include <QtGui/QWidget>
#include "project.h"

namespace Ui {
    class TaskElement;
}

class TaskElement : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(TaskElement)
public:
    explicit TaskElement(Task* task, QWidget *parent = 0);
    virtual ~TaskElement();
    Task* task();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::TaskElement *m_ui;
};

#endif // TASKELEMENT_H
