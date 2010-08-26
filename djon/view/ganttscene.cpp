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
    QSize size = headerSizeHint();

    QLinearGradient grad(0, 0, 0, size.height());
    int dark = 210;
    grad.setColorAt(1, QColor(dark, dark, dark));
    grad.setColorAt(0.7, QColor(dark + 20, dark + 20, dark + 20));
    grad.setColorAt(0, Qt::white);
    QBrush background(grad);
    QPen pen(QColor(220, 220, 220));
    addRect(0, 0, _viewSizeWidth, size.height() + 1, pen, background)->setZValue(1);

    int textSize = 30;
    int margin = 15;
    int columnSize = 45;

    QDateTime startDate = *_startDate.addDays(-1).toQDateTime();
    for (int x = 0; x < _totalDays; x++) {
        QPen textPen(Qt::black);
        QGraphicsSimpleTextItem* text = addSimpleText(startDate.toString("dd-MMM"), QFont("Arial", 8));
        text->setPos(x * 45 + 2, 3);
        text->setVisible(true);
//        text->setPen(textPen);
        text->setZValue(1);
        startDate = startDate.addDays(1);
    }

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

    int daysToStart = _startDate.daysTo(*startDate);
    int x1 = daysToStart * _dayWidth;
    int y1 = _currentY + size.height() - 10;

    int x2 = x1 + (days * _dayWidth);
    int y2 = _currentY + size.height() - 6;

    QGraphicsItem* item = addRect(x1 - 4, y1, (x2 - x1) + 8, (y2 - y1), pblack, bblack);
    item->setZValue(1);
    QVector<QPointF> trian1;
    trian1 << QPointF(x1 - 4, y2);
    trian1 << QPointF(x1, y2 + 4);
    trian1 << QPointF(x1 + 4, y2);
    QPolygonF poly1(trian1);
    item = addPolygon(poly1, pblack, bblack);
    item->setZValue(1);

    QVector<QPointF> trian2;
    trian2 << QPointF(x2 - 4 , y2);
    trian2 << QPointF(x2, y2 + 4);
    trian2 << QPointF(x2 + 4, y2);
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

    QSize size = sizeHint(index);

    int bordermargin = (size.height() * .4) / 2;

    int daysToStart = _startDate.daysTo(*task->startDate());
    int x1 = daysToStart * _dayWidth;
    int y1 = _currentY + bordermargin;

    int x2 = x1 + (days * _dayWidth);
    int y2 = _currentY + size.height() - bordermargin;

    QLinearGradient grad(QPointF(x1, y1), QPointF(x1, y2));
    grad.setColorAt(0, QColor(0, 0, 150));
    grad.setColorAt(0.4, QColor(0, 0, 180));
    grad.setColorAt(1, QColor(200, 200, 255));
    QBrush b(grad);//QImage(":/img/task_bar.png"));//(QPixmap(":/img/task_bar.png"));
    QPen pen(QColor(0, 0, 150));

    QGraphicsItem* item = this->addRect(x1, y1, (x2 - x1), (y2 - y1), pen, b);
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
            barcolor = QColor(240, 240, 240);
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

    calcZoom();
    createHeader();
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
    _startDate = start->addDays(-1);
    _endDate = end->addDays(1);
    _totalDays = _startDate.daysTo(_endDate) + 5;
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
