#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "project.h"
#include "taskgrid.h"
#include <QSystemTrayIcon>

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
    void createTrayIcon();
    QSystemTrayIcon* m_sysTray;

private slots:
    void on_actionReset_All_Timers_triggered();
    void on_actionStop_Time_triggered();
    void on_actionStart_Time_triggered();
    void on_actionEdit_Task_triggered();
    void on_actionCreate_new_task_triggered();
    void on_trayClicked();
};

#endif // MAINWINDOW_H
