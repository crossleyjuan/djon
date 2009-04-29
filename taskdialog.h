#ifndef TASKDIALOG_H
#define TASKDIALOG_H

#include <QtGui/QDialog>
#include "project.h"

namespace Ui {
    class TaskDialog;
}

class TaskDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(TaskDialog)
public:
    explicit TaskDialog(Task* task = 0, QWidget *parent = 0);
    virtual ~TaskDialog();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::TaskDialog *m_ui;
    Task* m_task;

private slots:
    void on_buttonBox_accepted();
};

#endif // TASKDIALOG_H
