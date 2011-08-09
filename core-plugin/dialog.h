#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QComboBox>
#include "datamanager.h"

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
    void setupGrid();

    Ui::Dialog *ui;
    const Workspace* _workspace;
    DataManager _manager;


private slots:
    void on_pushButton_clicked();
    void on_dateEdit_dateChanged(QDate date);
    void on_pushButton_2_clicked();
};

#endif // DIALOG_H
