#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <vector>
#include <QStackedLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include "taskgrid.h"

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
    ui->centralWidget->setLayout(layout);
    layout->addWidget(m_grid);
    ui->actionStart_Time->setEnabled(true);
    ui->actionStop_Time->setEnabled(false);
    createTrayIcon();

    connect(ui->actionRefresh, SIGNAL(triggered()), m_grid, SLOT(updateGrid()));
}

MainWindow::~MainWindow()
{
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
    m_grid->currentTaskElement()->startTimeRecord();
    ui->actionStart_Time->setEnabled(false);
    ui->actionStop_Time->setEnabled(true);

    QIcon icon(":/clock-on.png"); // clock-off.svg
    m_sysTray->setIcon(icon);
}

void MainWindow::on_actionStop_Time_triggered()
{
    m_grid->currentTaskElement()->stopTimeRecord();
    m_grid->updateGrid();
    ui->actionStart_Time->setEnabled(true);
    ui->actionStop_Time->setEnabled(false);
    QIcon icon(":/clock-off.svg");
    m_sysTray->setIcon(icon);
}

void MainWindow::on_actionReset_All_Timers_triggered()
{
    QMessageBox box;
    box.setText("This will reset all the timers, are you sure?");
    box.setInformativeText("After this action you'll not be able to recover your times. The log times will not be removed.");
    box.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    box.setDefaultButton(QMessageBox::No);
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
}

