#ifndef TASKFILTER_H
#define TASKFILTER_H

#include <QtGui/QDialog>
#include <vector>
#include "template.h"

namespace Ui {
    class TaskFilter;
}

class TaskFilter : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(TaskFilter)
public:
    explicit TaskFilter(QWidget *parent = 0);
    virtual ~TaskFilter();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::TaskFilter *m_ui;
    void populateTemplate();
    vector<Template*>* m_templates;

private slots:
    void populateStatus();
};

#endif // TASKFILTER_H
