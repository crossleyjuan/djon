#include "ganttchart.h"
#include "ui_ganttchart.h"
#include <QPainter>

GanttChart::GanttChart(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::GanttChart)
{
    m_ui->setupUi(this);
    m_taskHeight = 0;
    m_startDate = NULL;
    m_endDate = NULL;
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

void GanttChart::addTask(Task* task) {
    m_tasks.push_back(task);
}

void GanttChart::drawBackground(QPaintEvent* evt) {
    QPainter* p = new QPainter(this);

    p->setBrush(QBrush(QColor("white")));
    p->setPen(QPen(QColor("white")));
    p->drawRect(evt->rect());

    int columnSize = rect().width() / NUM_COLS;

    for (int x = 0; x < NUM_COLS; x++) {
        QColor barcolor;
        if ((x % 2) > 0) {
            barcolor = QColor("white");
        } else {
            barcolor = QColor(235, 235, 235);
        }
        p->setBrush(QBrush(barcolor));
        p->setPen(QPen(barcolor));
        p->drawRect(x*columnSize, HEADER_HEIGHT, (x+1)*columnSize, evt->rect().height());

        QPen pen(QColor(200, 200, 200));
        pen.setStyle(Qt::DashLine);
        p->setPen(pen);
        p->drawLine(x*columnSize, HEADER_HEIGHT, x*columnSize, evt->rect().height());
    }
}

bool GanttChart::hasTasks() {
    if (m_tasks.size() == 0) {
        return false;
    } else {
        return true;
    }
}

void GanttChart::calcZoom() {
    if (!hasTasks()) {
        return;
    }
    for (std::vector<Task*>::iterator it = m_tasks.begin(); it != m_tasks.end(); it++) {
        Task* tsk = *it;
        if ((m_startDate == NULL) || (*m_startDate > *(tsk->startDate()))) {
            m_startDate = tsk->startDate();
        }
        if ((m_endDate == NULL) || (*m_endDate < *(tsk->endDate()))) {
            m_endDate = tsk->endDate();
        }
    }
    m_totalDays = m_startDate->daysTo(*m_endDate) + 1;
    if ((m_totalDays > 1) && (m_totalDays < 8)) {
        m_scale = DAY;
    } else if ((m_totalDays > 7) && (m_totalDays < 16)) {
        m_scale = HALF_MONTH;
    } else if (m_totalDays > 15) {
        m_scale = MONTH;
    }
}

void GanttChart::drawHeader(QPaintEvent* evt) {
}

void GanttChart::setTaskHeight(int height) {
    m_taskHeight = height;
}

void GanttChart::drawTasks(QPaintEvent* evt) {
    QPainter* p = new QPainter(this);

    int dayWidth = evt->rect().width() / m_totalDays;
    int row = 0;
    int margin = (m_taskHeight * .3) / 2;

    for (std::vector<Task*>::iterator iter = m_tasks.begin(); iter != m_tasks.end(); iter++) {
        Task* task = *iter;
        int days = task->startDate()->daysTo(*task->endDate()) + 1;
        p->setPen(QPen(QColor("red")));
        p->setBrush(QBrush(QColor("red")));

        int daysToStart = m_startDate->daysTo(*task->startDate());
        int x1 = daysToStart*dayWidth;
        int y1 = (row * m_taskHeight) + HEADER_HEIGHT + margin;

        int x2 = x1 + (days*dayWidth);
        int y2 = ((row+1) * m_taskHeight) + HEADER_HEIGHT - margin;

        p->drawRect(x1, y1, (x2 - x1), (y2 - y1));
        row++;
    }
}

void GanttChart::paintEvent(QPaintEvent* evt) {
    calcZoom();

    if (m_taskHeight == 0) {
        m_taskHeight = 30;
    }
    drawBackground(evt);

    drawHeader(evt);

    drawTasks(evt);
}
