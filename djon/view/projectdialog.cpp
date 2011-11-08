// *********************************************************************************************************************
// file:
// author: Juan Pablo Crossley (crossleyjuan@gmail.com)
// created:
// updated:
// license:
// 
// This file is part of the djon project, all the source code is licensed under the terms of the GPL license,
// the application and libraries are provided as-is and free of use under the terms explained in the file COPYING.GPL
// the authors are not responsible for any problem that this program, or any of its libraries, may cause.
// Its authors create this application in order to make the world a better place to live, but you should use it on
// your own risks.
// 
// Also, be adviced that, the GPL license force the committers to ensure this application will be free of use, thus
// if you do any modification you will be required to provide it for free unless you use it for personal use (you may 
// charge yourself if you want), bare in mind that you will be required to provide a copy of the license terms that ensures
// this program will be open sourced and all its derivated work will be too.
// *********************************************************************************************************************

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
