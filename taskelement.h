#ifndef TASKELEMENT_H
#define TASKELEMENT_H

#include <QtGui/QWidget>
#include <QFocusEvent>
#include "project.h"
#include <QDateTime>
#include <QPainter>
#include "logtime.h"

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
    void resetCurrentTimer();

signals:
    void taskFocus(TaskElement* task);

protected:
    virtual void changeEvent(QEvent *e);
    bool eventFilter( QObject *obj, QEvent *ev );
    void paintEvent(QPaintEvent *p);


private:
    Ui::TaskElement *m_ui;
    Project* m_project;
    Task* m_task;
    LogTime* m_logTime;
    QTimer* m_timer;
    void refreshTime();
    bool m_timeRunning;


private slots:
    void on_txtDuration_editingFinished();
    void on_txtShort_editingFinished();
    void timeout();
};

#endif // TASKELEMENT_H
