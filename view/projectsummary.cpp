#include "projectsummary.h"
#include "ui_projectsummary.h"
#include <sstream>

ProjectSummary::ProjectSummary(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::ProjectSummary)
{
    ui->setupUi(this);
}

ProjectSummary::~ProjectSummary()
{
    delete ui;
}

void ProjectSummary::changeEvent(QEvent *e)
{
    QWizardPage::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void ProjectSummary::initializePage() {
    std::stringstream ss;
    ss << "Project Name: " << field("projectName").toString().toStdString() << "\n";
    ss << "Project Description: " << field("projectDescription").toString().toStdString() << "\n";

    ui->summary->setText(tr(ss.str().c_str()));
}
