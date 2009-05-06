#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "project.h"
#include "taskgrid.h"

namespace Ui
{
    class MainWindowClass;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Project* project, QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindowClass *ui;
    Project* m_project;
    TaskGrid* m_grid;

private slots:
    void on_actionCreate_new_task_triggered();
};

#endif // MAINWINDOW_H
