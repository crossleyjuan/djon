#ifndef PROJECTSUMMARY_H
#define PROJECTSUMMARY_H

#include <QWizardPage>

namespace Ui {
    class ProjectSummary;
}

class ProjectSummary : public QWizardPage {
    Q_OBJECT
public:
    ProjectSummary(QWidget *parent = 0);
    ~ProjectSummary();
    void initializePage();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ProjectSummary *ui;
};

#endif // PROJECTSUMMARY_H
