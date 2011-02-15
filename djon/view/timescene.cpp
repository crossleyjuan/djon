#include "timescene.h"
#include <QtGui>
#include "util.h"
#include "data.h"
#include "TaskModel.h"
#include <algorithm>

TimeScene::TimeScene(QObject *parent) :
        QGraphicsScene(parent)
{
    this->_model = NULL;
    initialize();
}

void TimeScene::initialize() {
    _currentX = 0;
    _currentY = 0;
    this->_dayWidth = 0;
    this->_totalDays = 0;
    this->_viewSizeHeight = 0;
    this->_viewSizeWidth = 0;
    clear();
}

void TimeScene::setModel(TaskModel* model) {
    initialize();
    _model = model;
    connect(_model, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(refresh()));
    setupScene();
}

void TimeScene::refresh() {
    initialize();
    setupScene();
}

void TimeScene::drawIndex(const QModelIndex &index) {
    if (!index.isValid()) {
        return;
    }
    drawTimeLog(index);
}

QSize TimeScene::sizeHint(const QModelIndex &index) {
    QVariant vSize = _model->data(index, Qt::SizeHintRole);
    QSize sizeHint = QSize(10, 10);
    if (vSize.canConvert<QSize>()) {
        sizeHint = qvariant_cast<QSize>(vSize);
    }
    return sizeHint;
}

bool TimeScene::compareTaskLog(TaskLog* log1, TaskLog* log2) {
    bool result = false;
    if ((log1 == NULL) && (log2 != NULL)) {
        result = false;
    } else if ((log1 != NULL) && (log2 == NULL)) {
        result = true;
    } else if ((log1 == NULL) && (log2 == NULL)) {
        result = (log1->id->compare(*log2->id) < 0);
    } else {
        result = (*log1->start < *log2->start);
    }

    return result;
}

void TimeScene::drawTime(DateTime currentDay, DTime time, QModelIndex index) {
    QSize size = sizeHint(index);
    int bordermargin = (size.height() * .1) / 2;

    QBrush b(Qt::white);
    QPen pen(QColor(0, 0, 150));

    int daysToStart = _startDate.daysTo(currentDay);
    int x1 = daysToStart * _dayWidth;
    int y1 = _currentY + bordermargin;
    int x2 = x1 + _dayWidth;
    int y2 = _currentY + size.height() - bordermargin;

    QGraphicsItem* item = this->addRect(x1, y1, (x2 - x1), (y2 - y1), pen, b);
    item->setZValue(1);
    QGraphicsSimpleTextItem* text = addSimpleText(QString(time.toChar()), QFont("Arial", 8));
    text->setPos(x1 + 2, y1 + 1);
    text->setVisible(true);
    text->setZValue(1);
}

void TimeScene::drawTimeLog(const QModelIndex &index) {
    Task* task = _model->task(index);

    if (task != NULL) {
        std::vector<TaskLog*> *logs = task->logs();
        if (logs->size() != 0) {
            std::sort(logs->begin(), logs->end(), compareTaskLog);

            DateTime* currentDay = NULL;
            DTime currentTime(0);

            for (std::vector<TaskLog*>::iterator iter = logs->begin(); iter != logs->end(); iter++) {
                TaskLog* log = *iter;
                DateTime logDate(log->start->getYear(), log->start->getMonth(), log->start->getDay());
                if (currentDay == NULL) {
                    currentDay = new DateTime(logDate.toDouble());
                    currentTime = (*log->end - *log->start);
                } else if (logDate == *currentDay) {
                    currentTime = currentTime + (*log->end - *log->start);
                } else {
                    drawTime(*currentDay, currentTime, index);
                    currentDay = new DateTime(logDate.toDouble());
                    currentTime = (*log->end - *log->start);
                }
            }
            if (currentTime.totalSecs() != 0) {
                drawTime(*currentDay, currentTime, index);
            }
        }
    }
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

void TimeScene::createBackground() {
    QBrush brush(QColor("white"));
    QPen pen(QColor("white"));
    // Background color
    QList<QGraphicsView*> views = this->views();
    int maxWidth = 0;
    int maxHeight = 0;
    for (QList<QGraphicsView*>::iterator iter = views.begin(); iter != views.end(); iter++) {
        QGraphicsView* view = *iter;
        int viewHeight = view->height();
        int viewWidth = view->width();
        if (viewHeight > maxHeight) {
            maxHeight = viewHeight;
        }
        if (viewWidth > maxWidth) {
            maxWidth = viewWidth;
        }
    }
    if (_viewSizeHeight > maxHeight) {
        maxHeight = _viewSizeHeight;
    }
    if (_viewSizeWidth > maxWidth) {
        maxWidth = _viewSizeWidth;
    }
    _viewSizeHeight = maxHeight;
    _viewSizeWidth = maxWidth;

    this->addRect(0, 0, maxWidth, maxHeight, pen, brush);

    //    int columnSize = geometry().width() / NU_COLS;
    int textSize = 30;
    int margin = 15;
    int cols = _totalDays;

    int columnSize = textSize + margin;

    if ((cols * columnSize) < maxWidth) {
        cols = (maxWidth / columnSize) + 10;
        _totalDays = cols;
    }
    DateTime startDate = this->_startDate;
    DateTime today;
    today.setHour(0);
    today.setMin(0);
    today.setSecs(0);
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
        int heigth = maxHeight;
        addRect(left, top, columnSize, heigth, penBar, brushBar);

        QPen pen(QColor(200, 200, 200));
        pen.setStyle(Qt::DashLine);

        addLine(x*columnSize, 0 , x*columnSize, heigth, pen);
        if (startDate == today) {
            barcolor = QColor(230, 230, 250);
            addRect(left, top, columnSize, heigth, penBar, QBrush(barcolor));
        }
        startDate = startDate.addDays(1);
    }
}

void TimeScene::setupScene() {
    this->clear();

    calcZoom();
    int projects = _model->rowCount(QModelIndex());
    for (int x = 0; x < projects; x++) {
        QModelIndex index = _model->index(x, 0);
        drawIndex(index);
    }
    this->_viewSizeHeight = _currentY; // + headerSizeHint().height()
    createBackground();
    setSceneRect(0, 0, _viewSizeWidth, _viewSizeHeight);
}

void TimeScene::calcZoom() {
    QModelIndex summaryIndex = _model->index(0, 0, QModelIndex());
    int projCount = _model->rowCount(summaryIndex);

    DateTime* start = NULL;
    DateTime* end = NULL;
    _viewSizeHeight = 0;
    _viewSizeWidth = 0;
    _dayWidth = 45;
    for (int x = 0; x < projCount; x++) {
        QModelIndex pIndex = _model->index(x, 0, summaryIndex);
        Project* proj = _model->project(pIndex);

        if ((proj != NULL) && pIndex.isValid()) {
            DateTime* pStart = proj->startDate();
            if (pStart != NULL) {
                DateTime* pEnd = proj->endDate();
                if ((start == NULL) || (*pStart < *start)) {
                    start = pStart;
                }
                if ((end == NULL) || (*pEnd > *end)) {
                    end = pEnd;
                }
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
    if (start != NULL) {
        _startDate = start->addDays(-1);
        _endDate = end->addDays(1);
    } else {
        _startDate = DateTime::today();
        _endDate = _startDate.addDays(1);
    }
    _totalDays = _startDate.daysTo(_endDate) + 5;
    this->_viewSizeWidth = _totalDays * _dayWidth;
}

void TimeScene::collapse(const QModelIndex& index) {
    _collapsedIndexes.push_back(index);
    refresh();
}

void TimeScene::expand(const QModelIndex& index) {
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

bool TimeScene::isCollapsed(const QModelIndex &index) {
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

QSize TimeScene::viewSizeHint() {
    return QSize(_viewSizeWidth, _viewSizeHeight);
}

DateTime TimeScene::startDate() {
    return _startDate;
}

DateTime TimeScene::endDate() {
    return _endDate;
}

int TimeScene::dayWidth() {
    return _dayWidth;
}

int TimeScene::totalDays() {
    return _totalDays;
}

