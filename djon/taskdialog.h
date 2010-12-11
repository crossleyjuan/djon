#ifndef TASKDIALOG_H
#define TASKDIALOG_H

#include <QtGui/QDialog>
#include "data.h"
#include "template.h"

namespace Ui {
    class TaskDialog;
}

class TaskDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(TaskDialog)
public:
    explicit TaskDialog(Project* project, Task* task = 0, QWidget *parent = 0);
    explicit TaskDialog(Project* project, string* id, QWidget *parent = 0);
    virtual ~TaskDialog();

    Task* task();

protected:
    virtual void changeEvent(QEvent *e);

private:
    void populateTemplate();
    void save();

    Ui::TaskDialog *m_ui;
    Task* m_task;
    Project* m_project;
    vector<Template*>* m_templates;
    string* _id;
    bool _update; // true if it's updating, false if it's creating

public slots:
    void done(int);

private slots:
    void on_buttonBox_rejected();
    void on_buttonBox_accepted();
    void populateStatus();
    void refreshEndDate();
};

#endif // TASKDIALOG_H
