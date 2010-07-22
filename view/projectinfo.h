#ifndef PROJECTINFO_H
#define PROJECTINFO_H

#include <QWizardPage>

namespace Ui {
    class ProjectInfo;
}

class ProjectInfo : public QWizardPage {
    Q_OBJECT
public:
    ProjectInfo(QWidget *parent = 0);
    ~ProjectInfo();
    void initializePage();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ProjectInfo *ui;
};

#endif // PROJECTINFO_H
