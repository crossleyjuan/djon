#ifndef TASKELEMENT_H
#define TASKELEMENT_H

#include <QtGui/QWidget>
#include <QFocusEvent>
#include "project.h"
#include <QDateTime>

namespace Ui {
    class TaskElement;
}

class TaskElement : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(TaskElement)
public:
    explicit TaskElement(Project* project, Task* task, QWidget *parent = 0);
    virtual ~TaskElement();
    Task* task();
    void startTimeRecord();
    void stopTimeRecord();

signals:
    void taskFocus(TaskElement* task);

protected:
    virtual void changeEvent(QEvent *e);
    bool eventFilter( QObject *obj, QEvent *ev );


private:
    Ui::TaskElement *m_ui;
    Project* m_project;
    Task* m_task;


private slots:
    void on_txtDuration_editingFinished();
    void on_txtShort_editingFinished();
};

#endif // TASKELEMENT_H
