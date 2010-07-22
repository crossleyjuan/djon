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
    QString projectName = field("projectName").toString();
    QString projectDescription = field("projectDescription").toString();
    _project->setName(new string(projectName.toStdString()));
    _project->setDescription(new string(projectDescription.toStdString()));
}
