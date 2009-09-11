#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <vector>
#include <QStackedLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include "taskgrid.h"
#include "taskfilter.h"

#include "taskdialog.h"

using namespace std;

MainWindow::MainWindow(Project* project, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindowClass)
{
    m_project = project;
    ui->setupUi(this);
    showMaximized();
    this->setWindowTitle(m_project->name.c_str());
    QStackedLayout* layout = new QStackedLayout();
    m_grid = new TaskGrid(m_project);
    m_grid->setTimerEnabled(false);
    ui->centralWidget->setLayout(layout);
    layout->addWidget(m_grid);
    createTrayIcon();

    updateState(false);

    connect(ui->actionRefresh, SIGNAL(triggered()), m_grid, SLOT(updateGrid()));

    idleDetector = new IdleDetector(10*60);// 5*60
    connect(idleDetector, SIGNAL(idleTimeOut()), this, SLOT(on_idleTimeOut()));
}

MainWindow::~MainWindow()
{
    idleDetector->stop();
    delete ui;
}

void MainWindow::on_actionCreate_new_task_triggered()
{
    TaskDialog* dialog = new TaskDialog(m_project, this);
    if (dialog->exec() == QDialog::Accepted) {
        m_grid->updateGrid();
    }
}


void MainWindow::on_actionEdit_Task_triggered()
{
    TaskDialog* dialog = new TaskDialog(m_project, m_grid->currentTaskElement()->task(), this);
    if (dialog->exec() == QDialog::Accepted) {
        m_grid->updateGrid();
    }
}

void MainWindow::on_actionStart_Time_triggered()
{
    idleDetector->start();

    m_grid->setTimerEnabled(true);

    updateState(true);
}

void MainWindow::on_actionStop_Time_triggered()
{
    idleDetector->stop();

    m_grid->setTimerEnabled(false);

    updateState(false);
}

void MainWindow::on_idleTimeOut() {
    idleDetector->stop();
    m_grid->currentTaskElement()->stopTimeRecord();
    //m_grid->updateGrid();
    m_grid->currentTaskElement()->startTimeRecord();

    QMessageBox box;
    box.setText("You've been idle more than 5 minutes, do you want to count that time?");
    box.setInformativeText("Don't be lazy!!!!");
    box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    box.setDefaultButton(QMessageBox::Yes);
    int res = box.exec();
    if (res == QMessageBox::No) {
        m_grid->currentTaskElement()->resetCurrentTimer();
//        updateState(false);
//    } else {
//        updateState(true);
    }
    idleDetector->start();
}

void MainWindow::updateState(bool timeRunning) {
    m_timeRunning = timeRunning;
    if (m_timeRunning) {
        QIcon icon(":/clock-on.png"); // clock-off.svg
        m_sysTray->setIcon(icon);
    } else {
        QIcon icon(":/clock-off.svg"); // clock-off.svg
        m_sysTray->setIcon(icon);
    }
    ui->actionStart_Time->setEnabled(!m_timeRunning);
    ui->actionStop_Time->setEnabled(m_timeRunning);
    ui->actionReset_Time->setEnabled(m_timeRunning);
}

void MainWindow::on_actionReset_All_Timers_triggered()
{
    QMessageBox box;
    box.setText("This will reset all the timers, are you sure?");
    box.setInformativeText("After this action you'll not be able to recover your times. The log times will not be removed.");
    box.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    box.setDefaultButton(QMessageBox::Cancel);
    int res = box.exec();
    if (res == QMessageBox::Yes) {
        resetTimes(m_project);
    }
    m_grid->updateGrid();
}

void MainWindow::createTrayIcon() {
    m_sysTray = new QSystemTrayIcon(this);
    QIcon icon(":/clock-off.svg");
    m_sysTray->setIcon(icon);
    connect(m_sysTray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(on_trayClicked()));
    m_sysTray->show();
}

void MainWindow::on_trayClicked() {
    this->showMaximized();
    this->activateWindow();
}

void MainWindow::on_actionReset_Time_triggered()
{
    QMessageBox box;
    box.setText("The current recorded time will be lost, are you sure?");
    box.setInformativeText("Don't be lazy!!!!");
    box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    box.setDefaultButton(QMessageBox::No);
    int res = box.exec();
    if (res == QMessageBox::Yes) {
//        idleDetector->stop();

        m_grid->currentTaskElement()->resetCurrentTimer();
        m_grid->updateGrid();

        updateState(false);
    }
}

void MainWindow::on_actionFilter_Tasks_triggered()
{
    TaskFilter* filter = new TaskFilter(this);
    int res = filter->exec();
    if (res == QDialog::Accepted) {

    }
}
