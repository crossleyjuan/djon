#include "projectinfo.h"
#include "ui_projectinfo.h"

ProjectInfo::ProjectInfo(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::ProjectInfo)
{
    ui->setupUi(this);
}

ProjectInfo::~ProjectInfo()
{
    delete ui;
}

void ProjectInfo::changeEvent(QEvent *e)
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

void ProjectInfo::initializePage() {
    registerField("projectName*", ui->projectName);
    registerField("projectDescription", ui->projectDescription, "plainText", SIGNAL(textChanged()));
}
