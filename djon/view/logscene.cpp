#include "logscene.h"
#include <QtGui>
#include "util.h"
#include "data.h"
#include "TaskModel.h"

LogScene::LogScene(QObject *parent) :
        QGraphicsScene(parent)
{
    this->_model = NULL;
    initialize();
}

void LogScene::initialize() {
    _currentX = 0;
    _currentY = 0;
    this->_dayWidth = 0;
    this->_totalDays = 0;
    this->_viewSizeHeight = 0;
    this->_viewSizeWidth = 0;
    clear();
}

void LogScene::setModel(TaskModel* model) {
    initialize();
    _model = model;
    connect(_model, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(refresh()));
    setupScene();
}

void LogScene::refresh() {
    initialize();
    setupScene();
}

void LogScene::drawIndex(const QModelIndex &index) {
    if (!index.isValid()) {
        return;
    }
    if (_model->rowCount(index) > 0) {
        getGroupItem(index);
    } else {
        Task* task = _model->task(index);
        if (task != NULL) {
            getTaskItem(index);
        }
    }
}

QSize LogScene::sizeHint(const QModelIndex &index) {
    QVariant vSize = _model->data(index, Qt::SizeHintRole);
    QSize sizeHint = QSize(10, 10);
    if (vSize.canConvert<QSize>()) {
        sizeHint = qvariant_cast<QSize>(vSize);
    }
    return sizeHint;
}

QGraphicsItem* LogScene::getGroupItem(const QModelIndex &index) {
//    Project* project = _model->project(index);
//    Task* task = _model->task(index);

//    DateTime* startDate = NULL;
//    DateTime* endDate = NULL;
//    if (task != NULL) {
//        startDate = task->startDate();
//        endDate = task->endDate();
//    } else {
//        if (project != NULL) {
//            startDate = project->startDate();
//            endDate = project->endDate();
//        } else {
//            startDate = &_startDate;
//            endDate = &_endDate;
//        }
//    }
//    // Only print group header for projects (not for summary)
//    if (project != NULL) {
//        int days = startDate->daysTo(*endDate) + 1;

//        QPen pblack(QColor("black"));
//        QBrush bblack(QColor("black"));

//        QSize size = sizeHint(index);

//        int daysToStart = _startDate.daysTo(*startDate);
//        int x1 = daysToStart * _dayWidth;
//        int y1 = _currentY + size.height() - 10;

//        int x2 = x1 + (days * _dayWidth);
//        int y2 = _currentY + size.height() - 6;

//        QGraphicsItem* item = addRect(x1 - 4, y1, (x2 - x1) + 8, (y2 - y1), pblack, bblack);
//        item->setZValue(1);
//        QVector<QPointF> trian1;
//        trian1 << QPointF(x1 - 4, y2);
//        trian1 << QPointF(x1, y2 + 4);
//        trian1 << QPointF(x1 + 4, y2);
//        QPolygonF poly1(trian1);
//        item = addPolygon(poly1, pblack, bblack);
//        item->setZValue(1);

//        QVector<QPointF> trian2;
//        trian2 << QPointF(x2 - 4 , y2);
//        trian2 << QPointF(x2, y2 + 4);
//        trian2 << QPointF(x2 + 4, y2);
//        QPolygonF poly2(trian2);

//        item = addPolygon(poly2, pblack, bblack);
//        item->setZValue(1);

//    }
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

QGraphicsItem* LogScene::getTaskItem(const QModelIndex &index) {
    Task* task = _model->task(index);

    Calendar* calendar = task->project()->projectDefaultCalendar();
    int hoursInDay = calendar->endHour().hour() - calendar->startHour().hour();
    std::vector<TaskLog*>* logs =task->logs();
    int startHour = calendar->startHour().hour();
    int endHour = calendar->endHour().hour() + 1;
    int minuteSize = 1000 / ((endHour - startHour) * 60);
    for (std::vector<TaskLog*>::iterator iter = logs->begin(); iter != logs->end(); iter++) {
        TaskLog* log = *iter;

        QSize size = sizeHint(index);

        int bordermargin = (size.height() * .4) / 2;

        int daysToStart = _startDate.daysTo(*log->start);
        int x1 = daysToStart * _dayWidth;
        DTime logStartTime = log->start->time();
        int y1 = logStartTime.minutes() * minuteSize;

        int x2 = (daysToStart + 1) * _dayWidth;
        DTime logEndTime = log->end->time();
        int y2 = logEndTime.minutes() * minuteSize;

        QLinearGradient grad(QPointF(x1, y1), QPointF(x1, y2));
        grad.setColorAt(0, QColor(0, 0, 150));
        grad.setColorAt(0.4, QColor(0, 0, 180));
        grad.setColorAt(1, QColor(200, 200, 255));
        QBrush b(grad);//QImage(":/img/task_bar.png"));//(QPixmap(":/img/task_bar.png"));
        QPen pen(QColor(0, 0, 150));

        QGraphicsItem* item = this->addRect(x1, y1, (x2 - x1), (y2 - y1), pen, b);
        QGraphicsSimpleTextItem* text = this->addSimpleText(tr(task->shortDescription()->c_str()));
        text->rotate(90);
        text->setPos(x1 + (_dayWidth / 2), y1);
        text->setVisible(true);
        QFont f("Arial", 8);
        f.setWeight(QFont::Light);

        QBrush brush(QColor(Qt::red));
        text->setBrush(brush);
        text->setFont(f);
        text->setZValue(2);
        item->setZValue(1);
        _currentY += sizeHint(index).height();
        return item;
    }
}

void LogScene::createBackground() {
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

void LogScene::setupScene() {
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

void LogScene::calcZoom() {
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

void LogScene::collapse(const QModelIndex& index) {
    _collapsedIndexes.push_back(index);
    refresh();
}

void LogScene::expand(const QModelIndex& index) {
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

bool LogScene::isCollapsed(const QModelIndex &index) {
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

QSize LogScene::viewSizeHint() {
    return QSize(_viewSizeWidth, _viewSizeHeight);
}

DateTime LogScene::startDate() {
    return _startDate;
}

DateTime LogScene::endDate() {
    return _endDate;
}

int LogScene::dayWidth() {
    return _dayWidth;
}

int LogScene::totalDays() {
    return _totalDays;
}

