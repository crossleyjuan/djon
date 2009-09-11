#include "taskfilter.h"
#include "ui_taskfilter.h"
#include "utils.h"

TaskFilter::TaskFilter(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::TaskFilter)
{
    m_ui->setupUi(this);

    populateTemplate();
    populateStatus();
}

TaskFilter::~TaskFilter()
{
    delete m_ui;
}

void TaskFilter::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void TaskFilter::populateTemplate() {
    m_templates = readTemplates();
    for (vector<Template*>::iterator iter = m_templates->begin(); iter != m_templates->end(); iter++) {
        Template* temp = *iter;
        m_ui->cboTemplate->addItem(QString(temp->name().c_str()));
    }

    connect(m_ui->cboTemplate, SIGNAL(currentIndexChanged(int)), this, SLOT(populateStatus()));
}

void TaskFilter::populateStatus() {
    m_ui->cboStatus->clear();
    Template* tpl = (Template*)m_templates->at(m_ui->cboTemplate->currentIndex());
    vector<string>* statusList = tpl->statusList();
    for (vector<string>::iterator iter = statusList->begin(); iter != statusList->end(); iter++) {
        string status = *iter;
        m_ui->cboStatus->addItem(QString(status.c_str()).trimmed());
    }

}

