#ifndef PROJECTWIZARD_H
#define PROJECTWIZARD_H

#include <QWizard>

class Project;

namespace Ui {
    class ProjectWizard;
}

class ProjectWizard : public QWizard {
    Q_OBJECT
public:
    ProjectWizard(QWidget *parent = 0);
    ~ProjectWizard();
    Project* project();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ProjectWizard *ui;
    Project* _project;

private slots:
    void on_ProjectWizard_finished(int result);
};

#endif // PROJECTWIZARD_H
