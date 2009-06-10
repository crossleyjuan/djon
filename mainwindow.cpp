#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <vector>
#include <QStackedLayout>
#include <QVBoxLayout>
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
}

void MainWindow::on_actionStop_Time_triggered()
{
    m_grid->currentTaskElement()->stopTimeRecord();
    m_grid->updateGrid();
    ui->actionStart_Time->setEnabled(true);
    ui->actionStop_Time->setEnabled(false);
}
