#include "ganttchart.h"
#include "ui_ganttchart.h"

GanttChart::GanttChart(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::GanttChart)
{
    m_ui->setupUi(this);
}

GanttChart::~GanttChart()
{
    delete m_ui;
}

void GanttChart::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void GanttChart::paintEvent(QPai
