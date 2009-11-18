#ifndef TASKDIALOG_H
#define TASKDIALOG_H

#include <QtGui/QDialog>
#include "project.h"
#include "template.h"

namespace Ui {
    class TaskDialog;
}

class TaskDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(TaskDialog)
public:
    explicit TaskDialog(Project* project, Task* task = 0, QWidget *parent = 0);
    explicit TaskDialog(Project* project, QWidget *parent = 0);
    virtual ~TaskDialog();

signals:
    void taskChanged(Task* task);


protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::TaskDialog *m_ui;
    Task* m_task;
    Project* m_project;
    void populateTemplate();
    void populateTableLog();
    vector<Template*>* m_templates;

private slots:
    void on_buttonBox_rejected();
    void on_buttonBox_accepted();
    void populateStatus();
};

#endif // TASKDIALOG_H
