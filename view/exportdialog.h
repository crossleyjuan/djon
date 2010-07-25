#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include <QDialog>

namespace Ui {
    class ExportDialog;
}

class ExportDialog : public QDialog {
    Q_OBJECT
public:
    ExportDialog(QWidget *parent = 0);
    ~ExportDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ExportDialog *ui;
};

#endif // EXPORTDIALOG_H
