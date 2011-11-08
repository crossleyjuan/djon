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

#include "projectwizard.h"
#include "ui_projectwizard.h"
#include "data.h"
#include "projectinfo.h"
#include "projectsummary.h"
#include <sstream>
#include <QtGui>

ProjectWizard::ProjectWizard(QWidget *parent) :
        QWizard(parent),
        ui(new Ui::ProjectWizard)
{
    ui->setupUi(this);
    _project = NULL;
    addPage(new ProjectInfo(this));
    addPage(new ProjectSummary(this));
    connect(this, SIGNAL(finished(int)), this, SLOT(on_ProjectWizard_finished(int)));
}

ProjectWizard::~ProjectWizard()
{
    delete ui;
}

Project* ProjectWizard::project() {
    return _project;
}

void ProjectWizard::changeEvent(QEvent *e)
{
    QWizard::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void ProjectWizard::on_ProjectWizard_finished(int result)
{
    _project = new Project();
    _project->setId(uuid());
    QString projectName = field("projectName").toString();
    QString projectDescription = field("projectDescription").toString();
    _project->setName(new string(projectName.toStdString()));
    _project->setDescription(new string(projectDescription.toStdString()));
    _project->setProjectFileName(new string(projectName.toStdString()));
}
