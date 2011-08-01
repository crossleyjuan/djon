#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    std::string homeDir = *getHomeDir() + "/.djon";
    std::string workspaceFile = homeDir + "/default.dwk";
    _workspace = loadWorkspace(workspaceFile);
    _plugin = new Coreplugin();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    _plugin->setWorkspace(_workspace);
    _plugin->showDefaultScreen(this);
}
