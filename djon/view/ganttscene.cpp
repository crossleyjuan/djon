#include "ganttscene.h"
#include <QtGui>
#include "util.h"
#include "data.h"
#include "TaskModel.h"

GanttScene::GanttScene(QObject *parent) :
    QGraphicsScene(parent)
{
    this->_model = NULL;
    initialize();
}

void GanttScene::initialize() {
    _currentX = 0;
    _currentY = 0;
    this->_dayWidth = 0;
    this->_endDate = NULL;
    this->_startDate = NULL;
    this->_totalDays = 0;
    this->_viewSizeHeight = 0;
    this->_viewSizeWidth = 0;
}

void GanttScene::setModel(TaskModel* model) {
    initialize();
    _model = model;
    connect(_model, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(refresh()));
    setupScene();
}

void GanttScene::refresh() {
    initialize();
    setupScene();
}

void GanttScene::createHeader() {
    _currentY += headerSizeHint().height();
}

void GanttScene::drawIndex(const QModelIndex &index) {
    if (_model->rowCount(index) > 0) {
        getGroupItem(index);
    } else {
        getTaskItem(index);
    }
}

QSize GanttScene::headerSizeHint() {
    QVariant vSize = _model->headerData(0, Qt::Vertical, Qt::SizeHintRole);
    QSize sizeHint = QSize(10, 19);
    if (vSize.canConvert<QSize>()) {
        sizeHint = qvariant_cast<QSize>(vSize);
    }
    return sizeHint;
}

QSize GanttScene::sizeHint(const QModelIndex &index) {
    QVariant vSize = _model->data(index, Qt::SizeHintRole);
    QSize sizeHint = QSize(10, 10);
    if (vSize.canConvert<QSize>()) {
        sizeHint = qvariant_cast<QSize>(vSize);
    }
    return sizeHint;
}

QGraphicsItem* GanttScene::getGroupItem(const QModelIndex &index) {
    Project* project = _model->project(index);
    Task* task = _model->task(index);

    DateTime* startDate;
    DateTime* endDate;
    if (task != NULL) {
        startDate = task->startDate();
        endDate = task->endDate();
    } else {
        startDate = project->startDate();
        endDate = project->endDate();
    }
    int days = startDate->daysTo(*endDate) + 1;

    QPen pblack(QColor("black"));
    QBrush bblack(QColor("black"));

    QSize size = sizeHint(index);

    int daysToStart = _startDate->daysTo(*startDate);
    int x1 = daysToStart * _dayWidth;
    int y1 = _currentY + size.height() - 10;

    int x2 = x1 + (days * _dayWidth);
    int y2 = _currentY + size.height() - 5;

    QGraphicsItem* item = addRect(x1, y1, (x2 - x1), (y2 - y1), pblack, bblack);
    item->setZValue(1);
    QVector<QPointF> trian1;
    trian1 << QPointF(x1, y1);
    trian1 << QPointF(x1 + 5, y1);
    trian1 << QPointF(x1, y1 + 10);
    QPolygonF poly1(trian1);
    item = addPolygon(poly1, pblack, bblack);
    item->setZValue(1);

    QVector<QPointF> trian2;
    trian2 << QPointF(x2, y1);
    trian2 << QPointF(x2 - 5, y1);
    trian2 << QPointF(x2, y1 + 10);
    QPolygonF poly2(trian2);

    item = addPolygon(poly2, pblack, bblack);
    item->setZValue(1);

    _currentY += sizeHint(index).height();
    if (!isCollapsed(index)) {
        for (int x = 0; x < _model->rowCount(index); x++) {
            QModelIndex child = _model->index(x, 0, index);
            if (child.isValid()) {
                drawIndex(child);
            }
        }
    }
}

QGraphicsItem* GanttScene::getTaskItem(const QModelIndex &index) {
    Task* task = _model->task(index);
    DateTime* barStartDate = task->startDate();
    DateTime* barEndDate = task->endDate();
    QDate* startDate = new QDate(barStartDate->getYear(), barStartDate->getMonth(), barStartDate->getDay());
    QDate* endDate = new QDate(barEndDate->getYear(), barEndDate->getMonth(), barEndDate->getDay());
    int days = startDate->daysTo(*endDate) + 1;

    QPen pen(QColor("blue"));
    QBrush brush(QColor("blue"));

    QSize size = sizeHint(index);

    int bordermargin = (15 * .5) / 2;

    int daysToStart = _startDate->daysTo(*task->startDate());
    int x1 = daysToStart * _dayWidth;
    int y1 = _currentY + bordermargin;

    int x2 = x1 + (days * _dayWidth);
    int y2 = _currentY + size.height() - bordermargin;

    QGraphicsItem* item = this->addRect(x1, y1, (x2 - x1), (y2 - y1), pen, brush);
    item->setZValue(1);
    _currentY += sizeHint(index).height();
    return item;
}

void GanttScene::createBackground() {
    QBrush brush(QColor("white"));
    QPen pen(QColor("white"));
    // Background color
    QGraphicsItem* item = this->addRect(0, 0, _viewSizeWidth, _viewSizeHeight, pen, brush);
    item->setZValue(0);

    //    int columnSize = geometry().width() / NU_COLS;
    int textSize = 30;
    int margin = 15;
    int cols = _totalDays;
    int columnSize = textSize + margin;

    for (int x = 0; x < (cols + 1); x++) {
        QColor barcolor;
        if ((x % 2) > 0) {
            barcolor = QColor("white");
        } else {
            barcolor = QColor(235, 235, 235);
        }
        QBrush brushBar(barcolor);
        QPen penBar(barcolor);
        int left = (x*columnSize) + 0;
        int top = 0;
        int heigth = _viewSizeHeight;
        item = addRect(left, top, columnSize, heigth, penBar, brushBar);
        item->setZValue(0);

        QPen pen(QColor(200, 200, 200));
        pen.setStyle(Qt::DashLine);

        item = addLine(x*columnSize, 0 , x*columnSize, heigth, pen);
        item->setZValue(0);
    }
}

void GanttScene::setupScene() {
    this->clear();

    createHeader();
    calcZoom();
    int projects = _model->rowCount(QModelIndex());
    for (int x = 0; x < projects; x++) {
        QModelIndex index = _model->index(x, 0);
        drawIndex(index);
    }
    this->_viewSizeHeight = _currentY; // + headerSizeHint().height()
    createBackground();
}

void GanttScene::calcZoom() {
    int projCount = _model->rowCount(QModelIndex());

    DateTime* start = NULL;
    DateTime* end = NULL;
    _viewSizeHeight = 0;
    _viewSizeWidth = 0;
    _dayWidth = 45;
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
    _totalDays = _startDate->daysTo(*_endDate);
    this->_viewSizeWidth = _totalDays * _dayWidth;
}

void GanttScene::collapse(const QModelIndex& index) {
    _collapsedIndexes.push_back(index);
    refresh();
}

void GanttScene::expand(const QModelIndex& index) {
    std::vector<QModelIndex>::iterator iter = _collapsedIndexes.begin();
    bool found = false;
    while (iter != _collapsedIndexes.end()) {
        QModelIndex curr = *iter;
        if (curr == index) {
            found = true;
            break;
        }
        iter++;
    }
    if (found) {
        _collapsedIndexes.erase(iter);
        refresh();
    }
}

bool GanttScene::isCollapsed(const QModelIndex &index) {
    std::vector<QModelIndex>::iterator iter = _collapsedIndexes.begin();
    bool found = false;
    while (iter != _collapsedIndexes.end()) {
        QModelIndex curr = *iter;
        if (curr == index) {
            found = true;
            break;
        }
        iter++;
    }
    return found;
}
