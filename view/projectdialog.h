#ifndef PROJECTDIALOG_H
#define PROJECTDIALOG_H

#include <QDialog>
class Project;

namespace Ui {
    class ProjectDialog;
}

class ProjectDialog : public QDialog {
    Q_OBJECT
public:
    ProjectDialog(Project* project, QWidget *parent = 0);
    ~ProjectDialog();
    void done(int res);
    Project* project();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ProjectDialog *ui;
    Project* _project;

private slots:
    void on_ProjectDialog_accepted();
};

#endif // PROJECTDIALOG_H
