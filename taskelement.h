#ifndef TASKELEMENT_H
#define TASKELEMENT_H

#include <QtGui/QWidget>
#include <QFocusEvent>
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

signals:
    void taskFocus(Task* task);

protected:
    virtual void changeEvent(QEvent *e);
    bool eventFilter( QObject *obj, QEvent *ev );


private:
    Ui::TaskElement *m_ui;
    Task* m_task;



};

#endif // TASKELEMENT_H
