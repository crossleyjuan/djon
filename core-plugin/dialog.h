#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
    class Dialog;
}

class Workspace;

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(const Workspace* workspace, QWidget *parent = 0);
    ~Dialog();

private:
    void showTasks();

    Ui::Dialog *ui;
    const Workspace* _workspace;
};

#endif // DIALOG_H
