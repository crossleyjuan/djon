#include "ganttscene.h"
#include <QtGui>
#include "util.h"
#include "data.h"
#include "TaskModel.h"

GanttScene::GanttScene(QObject *parent) :
    QGraphicsScene(parent)
{
    this->_model = NULL;
}

void GanttScene::setModel(TaskModel* model) {
    _model = model;
    setupScene();
}

QGraphicsItem* GanttScene::getTaskItem(Task* task) {
    DateTime* barStartDate = task->startDate();
    DateTime* barEndDate = task->endDate();
    QDate* startDate = new QDate(barStartDate->getYear(), barStartDate->getMonth(), barStartDate->getDay());
    QDate* endDate = new QDate(barEndDate->getYear(), barEndDate->getMonth(), barEndDate->getDay());
    int days = startDate->daysTo(*endDate) + 1;

    QPen pen(QColor("blue"));
    QBrush brush(QColor("blue"));

    int bordermargin = (15 * .5) / 2;
    int dayWidth = 45;

    int daysToStart = _startDate->daysTo(*task->startDate());
    int x1 = daysToStart*dayWidth;
    int y1 = bordermargin;

    int x2 = x1 + (days*dayWidth);
    int y2 = 15 - bordermargin;

    int left = 100;
    int top = 10;
    QGraphicsItem* item = this->addRect(x1 + left, y1 + top, (x2 - x1), (y2 - y1), pen, brush);
    return item;
}

void GanttScene::drawBackground(QPainter *painter, const QRectF &rect) {
    QPainter* p = painter;
    p->save();
    p->setBrush(QBrush(QColor("white")));
    p->setPen(QPen(QColor("white")));
    p->drawRect(rect);

    //    int columnSize = geometry().width() / NU_COLS;
    int textSize = 30;
    int margin = 15;
    int cols = (rect.width() / (textSize + margin));
    int columnSize = textSize + margin;

    p->setFont(QFont("Arial", 9));
    for (int x = 0; x < (cols + 1); x++) {
        QColor barcolor;
        if ((x % 2) > 0) {
            barcolor = QColor("white");
        } else {
            barcolor = QColor(235, 235, 235);
        }
        p->setBrush(QBrush(barcolor));
        p->setPen(QPen(barcolor));
        int left = (x*columnSize) + rect.left();
        int top = 0 + rect.top();
        int heigth = rect.height();
        p->drawRect(left, top, columnSize, heigth);

        QPen pen(QColor(200, 200, 200));
        pen.setStyle(Qt::DashLine);
        p->setPen(pen);
        p->drawLine(x*columnSize + rect.left(), 0 + rect.top(), x*columnSize + rect.left(), rect.height());
    }
    p->restore();
}

void GanttScene::setupScene() {
    this->items().clear();

    int projects = _model->rowCount(QModelIndex());
    for (int x = 0; x < projects; x++) {
        QModelIndex index = _model->index(x, 0);
        Task* task = _model->task(index);
        if (task != NULL) {
            getTaskItem(task);
        }
    }
}

void GanttScene::calcZoom() {
    int projCount = _model->rowCount(QModelIndex());

    DateTime* start = NULL;
    DateTime* end = NULL;
    for (int x = 0; x < projCount; x++) {
        QModelIndex pIndex = _model->index(x, 0, QModelIndex());
        Project* proj = _model->project(pIndex);
        if (pIndex.isValid()) {
            DateTime* pStart = proj->startDate();
            DateTime* pEnd = proj->endDate();
            if ((start == NULL) || (*pStart < *start)) {
                start = pStart;
            }
            if ((end == NULL) || (*pEnd > *end)) {
                end = pEnd;
            }
        }
    }
    /*
    if (start != NULL) {
        totalDays = start->daysTo(*end) + 1;
        if ((totalDays > 1) && (totalDays < 8)) {
            scale = DAY;
        } else if ((totalDays > 7) && (totalDays < 16)) {
            scale = HALF_MONTH;
        } else if (totalDays > 15) {
            scale = MONTH;
        }
    }
    */
    _startDate = start;
    _endDate = end;
}
