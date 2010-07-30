#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include <QDialog>
#include <vector>
#include "util.h"
class Project;

namespace Ui {
    class ExportDialog;
}

class ExportDialog : public QDialog {
    Q_OBJECT
public:
    ExportDialog(std::vector<Project*> projects, QWidget *parent = 0);
    ~ExportDialog();
    void done(int res);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ExportDialog *ui;
    std::vector<Project*> _projects;
    DateTime* _minDate;
    DateTime* _maxDate;

private slots:
    void on_cbFilter_toggled(bool checked);
    void on_ExportDialog_accepted();
    void on_pushButton_clicked();
};

#endif // EXPORTDIALOG_H
