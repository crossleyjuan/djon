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
    m_headerHeight = HEADER_HEIGHT;
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

void GanttChart::setHeaderHeight(int height) {
    m_headerHeight = height;
}

void GanttChart::addTask(GanttTask* task) {
    m_tasks.push_back(task);
}

void GanttChart::drawBackground() {
    QPainter p(this);

    p.setBrush(QBrush(QColor("white")));
    p.setPen(QPen(QColor("white")));
    p.drawRect(0, 0, width(), height());

//    int columnSize = geometry().width() / NUM_COLS;
    int textSize = 30;
    int margin = 15;
    int cols = (geometry().width() / (textSize + margin));
    int columnSize = textSize + margin;

    QDate startDate;
    int days = 0;
    if (m_startDate != NULL) {
        startDate = QDate(m_startDate->year(), m_startDate->month(), m_startDate->day());
        days = startDate.daysTo(*m_endDate) / cols;
    }

    p.setFont(QFont("Arial", 9));
    for (int x = 0; x < (cols + 1); x++) {
        QColor barcolor;
        if ((x % 2) > 0) {
            barcolor = QColor("white");
        } else {
            barcolor = QColor(235, 235, 235);
        }
        p.setBrush(QBrush(barcolor));
        p.setPen(QPen(barcolor));
        p.drawRect(x*columnSize, m_headerHeight, (x+1)*columnSize, this->rect().height());

        QPen pen(QColor(200, 200, 200));
        pen.setStyle(Qt::DashLine);
        p.setPen(pen);
        p.drawLine(x*columnSize, m_headerHeight, x*columnSize, this->rect().height());

        if (m_startDate != NULL) {
            p.setPen(QColor("black"));
            p.drawText(x*(textSize + margin), 10, startDate.toString("dd-MMM"));
            startDate = startDate.addDays(days);
        }
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
    for (std::vector<GanttTask*>::iterator it = m_tasks.begin(); it != m_tasks.end(); it++) {
        GanttTask* tsk = *it;
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

void GanttChart::setTaskHeight(int height) {
    m_taskHeight = height;
}

void GanttChart::clear() {
    m_tasks.clear();
}

void GanttChart::drawTasks() {
    QPainter p(this);

    int dayWidth = this->rect().width() / m_totalDays;
    int row = 0;
    int bordermargin = (m_taskHeight * .5) / 2;

    for (std::vector<GanttTask*>::iterator iter = m_tasks.begin(); iter != m_tasks.end(); iter++) {
        GanttTask* task = *iter;
        int days = task->startDate()->daysTo(*task->endDate());
        p.setPen(QPen(QColor("red")));
        p.setBrush(QBrush(QColor("red")));

        int daysToStart = m_startDate->daysTo(*task->startDate());
        int x1 = daysToStart*dayWidth;
        int y1 = (row * m_taskHeight) + m_headerHeight + bordermargin;

        int x2 = x1 + (days*dayWidth);
        int y2 = ((row+1) * m_taskHeight) + m_headerHeight - bordermargin;

        p.drawRect(x1, y1, (x2 - x1), (y2 - y1));
//        p.setPen(QColor("black"));
//        p.drawText(x1, y1, task->startDate()->toString("dd"));
        row++;
    }
}

void GanttChart::paintEvent(QPaintEvent* evt) {
    calcZoom();

    if (m_taskHeight == 0) {
        m_taskHeight = 30;
    }
    drawBackground();

    drawTasks();
}
