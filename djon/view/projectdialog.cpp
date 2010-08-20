#include "projectdialog.h"
#include "ui_projectdialog.h"

#include "data.h"
#include <QtGui>

ProjectDialog::ProjectDialog(Project* project, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProjectDialog)
{
    ui->setupUi(this);
    _project = project;
    ui->projectName->setText(QString(_project->name()->c_str()));
    ui->projectDescription->setText(QString(_project->description()->c_str()));

    connect(this, SIGNAL(accepted()), this, SLOT(on_ProjectDialog_accepted()));
}

ProjectDialog::~ProjectDialog()
{
    delete ui;
}

void ProjectDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void ProjectDialog::on_ProjectDialog_accepted()
{
    _project->setName(new string(ui->projectName->text().toStdString()));
    _project->setDescription(new string(ui->projectDescription->document()->toPlainText().toStdString()));
}

void ProjectDialog::done(int res) {
    if (res == 1) {
        if (ui->projectName->text().trimmed().size() == 0) {
            QMessageBox box;
            box.setWindowTitle(tr("d-jon"));
            box.setText("The project name is required");
            return;
        }
    }
    QDialog::done(res);
}

Project* ProjectDialog::project() {
    return _project;
}
