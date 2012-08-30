// *********************************************************************************************************************
// file:    timescene.cpp
// author:  Juan Pablo Crossley (crossleyjuan@gmail.com)
// created: Oct/1/2010
// updated: Nov/7/2011
// license: GPL 2
//
// This file is part of the djon project, all the source code is licensed under the terms of the GPL license,
// the application and libraries are provided as-is and free of use under the terms explained in the file COPYING.GPL
// the authors are not responsible for any problem that this program, or any of its libraries, may cause.
// Its authors create this application in order to make the world a better place to live, but you should use it on
// your own risks.
// 
// Also, be adviced that, the GPL license force the committers to ensure this application will be free of use, thus
// if you do any modification you will be required to provide it for free unless you use it for personal use (you may 
// charge yourself if you want), bare in mind that you will be required to provide a copy of the license terms that ensures
// this program will be open sourced and all its derivated work will be too.
// *********************************************************************************************************************

#include "timescene.h"
#include <QtGui>
#include "util.h"
#include "data.h"
#include "TaskModel.h"
#include "graphicsrectitem.h"
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

void TimeScene::drawTime(DateTime currentDay, DTime time, QModelIndex index, int groupLevel) {
    QSize size = sizeHint(index);
    int bordermargin = (size.height() * .1) / 2;

    QBrush b;
    QPen pen(QColor(0, 0, 150));
    QBrush textBrush;
    if (groupLevel == 2) {
        b = QBrush(Qt::white);
        pen = QPen(QColor(0, 0, 150));
        textBrush = QBrush(Qt::darkBlue);
    }
    if (groupLevel == 1) {
        b = QBrush(Qt::lightGray);
        pen = QPen(QColor(0, 0, 50));
        textBrush = QBrush(Qt::blue);
    }
    if (groupLevel == 0) {
        b = QBrush(Qt::darkGray);
        pen = QPen(QColor(100, 100, 200));
        textBrush = QBrush(Qt::white);
    }

    int daysToStart = _startDate.daysTo(currentDay);
    int x1 = daysToStart * _dayWidth;
    int y1 = _currentY + bordermargin;
    int x2 = x1 + _dayWidth;
    int y2 = _currentY + size.height() - bordermargin;

//    QGraphicsRectItem* item = this->addRect(x1, y1, (x2 - x1), (y2 - y1), pen, b);
    QRect rect(x1, y1, (x2 - x1), (y2 - y1));
    GraphicsRectItem* item = new GraphicsRectItem(rect, index);
    this->addItem(item);
    item->setPen(pen);
    item->setBrush(b);
    item->setZValue(1);
    item->setAcceptHoverEvents(true);

    connect(item, SIGNAL(itemHoverEnter(QModelIndex)), this, SLOT(receiveItemHoverEnter(QModelIndex)));
    connect(item, SIGNAL(itemHoverLeave(QModelIndex)), this, SLOT(receiveItemHoverLeave(QModelIndex)));

    QFont font("Arial", 8);

    font.setBold((groupLevel <= 1));
    font.setItalic((groupLevel == 0));

    QGraphicsSimpleTextItem* text = addSimpleText(time.toQString(), font);
    text->setBrush(textBrush);
    text->setPos(x1 + 2, y1 + 1);
    text->setVisible(true);
    text->setZValue(1);
}

void TimeScene::drawTimeLog(const QModelIndex &index) {
    Task* task = _model->task(index);

    std::vector<TaskLog*> *logs = NULL;
    int groupLevel = 2;
    if (task != NULL) {
        logs = task->logs(true);
        groupLevel = (task->childCount() > 0) ? 1 : 2;
    } else {
        Project* project = _model->project(index);
        if (project != NULL) {
            logs = project->logs();
            groupLevel = 0;
        }
    }
    if ((logs != NULL) && (logs->size() != 0)) {
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
                drawTime(*currentDay, currentTime, index, groupLevel);
                if (currentDay != NULL) {
                    delete(currentDay);
                    currentDay = NULL;
                }
                currentDay = new DateTime(logDate.toDouble());
                currentTime = (*log->end - *log->start);
            }
        }
        if (currentTime.totalSecs() != 0) {
            drawTime(*currentDay, currentTime, index, groupLevel);
        }
        if (currentDay != NULL) {
            delete(currentDay);
            currentDay = NULL;
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
    if (logs != NULL) {
        delete(logs);
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
            if ((pStart != NULL) && (*pStart > DateTime(0, 0, 0))) {
                DateTime* pEnd = proj->endDate();
                if ((start == NULL) || (*pStart < *start)) {
                    start = pStart;
                }
                if (*pEnd > DateTime(0, 0, 0)) {
                    if ((end == NULL) || (*pEnd > *end)) {
                        end = pEnd;
                    }
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
    // Ensures today will be shown in the graph
    if (_endDate < DateTime::today()) {
        _endDate = DateTime::today();
    }
    // Force to show at least 10 days, this will avoid the blank screen effect
    if (DateTime::today().daysTo(_endDate) < 30) {
        _endDate = DateTime::today().addDays(30);
    } else {
        _endDate = _endDate.addDays(3);
    }
    _totalDays = _startDate.daysTo(_endDate);
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

void TimeScene::receiveItemHoverEnter(QModelIndex index) {
    emit itemHoverEnter(index);
}

void TimeScene::receiveItemHoverLeave(QModelIndex index) {
    emit itemHoverLeave(index);
}
