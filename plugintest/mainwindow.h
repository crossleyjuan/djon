#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "data.h"
#include "coreplugin.h"


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    Workspace* _workspace;
    Coreplugin* _plugin;

private slots:
    void on_pushButton_clicked();
};

#endif // MAINWINDOW_H
