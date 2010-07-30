#include "taskdelegate.h"
#include "TaskModel.h"
#include "data.h"

TaskDelegate::TaskDelegate(QDate* startDate, QDate* endDate, int totalDays, SCALE scale)
{
    _startDate = startDate;
    _endDate = endDate;
    _totalDays = totalDays;
    _scale = scale;
}

void TaskDelegate::paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const {
    if (!index.isValid()) {
        return;
    }
    drawBackground(painter, option, index);
    drawTasks(painter, option, index);
}

QSize TaskDelegate::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const {
    return QSize(10, 17);
}

void TaskDelegate::drawBackground(QPainter* p, const QStyleOptionViewItem & option, const QModelIndex & index ) const {
    p->save();
    p->setBrush(QBrush(QColor("white")));
    p->setPen(QPen(QColor("white")));
    p->drawRect(option.rect);

    //    int columnSize = geometry().width() / NU_COLS;
    int textSize = 30;
    int margin = 15;
    int cols = (option.rect.width() / (textSize + margin));
    int columnSize = textSize + margin;

    QDate startDate;
    int days = 0;
    if (_startDate != NULL) {
        startDate = QDate(_startDate->year(), _startDate->month(), _startDate->day());
        days = startDate.daysTo(*_endDate) / cols;
    }

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
        int left = (x*columnSize) + option.rect.left();
        int top = 0 + option.rect.top();
        int width = columnSize;
        int heigth = option.rect.height();
        p->drawRect(left, top, columnSize, heigth);

        QPen pen(QColor(200, 200, 200));
        pen.setStyle(Qt::DashLine);
        p->setPen(pen);
        p->drawLine(x*columnSize + option.rect.left(), 0 + option.rect.top(), x*columnSize + option.rect.left(), option.rect.height());
    }
    p->restore();
}

void TaskDelegate::drawTaskBar(QPainter *p, DateTime* barStartDate, DateTime* barEndDate, int dayWidth, const QStyleOptionViewItem & option) const {
    QDate* startDate = new QDate(barStartDate->getYear(), barStartDate->getMonth(), barStartDate->getDay());
    QDate* endDate = new QDate(barEndDate->getYear(), barEndDate->getMonth(), barEndDate->getDay());
    int days = startDate->daysTo(*endDate) + 1;

    p->setPen(QPen(QColor("blue")));
    p->setBrush(QBrush(QColor("blue")));
    int bordermargin = (option.rect.height() * .5) / 2;

    int daysToStart = _startDate->daysTo(*startDate);
    int x1 = daysToStart*dayWidth;
    int y1 = bordermargin;

    int x2 = x1 + (days*dayWidth);
    int y2 = option.rect.height() - bordermargin;

    p->drawRect(x1 + option.rect.left(), y1 + option.rect.top(), (x2 - x1), (y2 - y1));
}

void TaskDelegate::drawGroupBar(QPainter *p, DateTime* barStartDate, DateTime* barEndDate, int dayWidth, const QStyleOptionViewItem & option) const {
    QDateTime* startDate = barStartDate->toQDateTime();
    QDateTime* endDate = barEndDate->toQDateTime();
    int days = startDate->daysTo(*endDate) + 1;

    p->setPen(QPen(QColor("black")));
    p->setBrush(QBrush(QColor("black")));

    int height = 3;

    int daysToStart = _startDate->daysTo(startDate->date());
    int x1 = daysToStart*dayWidth;
    int y1 = option.rect.height() - height -5;

    int x2 = x1 + (days*dayWidth);
    int y2 = option.rect.height() - 5;

    p->drawRect(x1 + option.rect.left(), y1 + option.rect.top(), (x2 - x1), (y2 - y1));
}

void TaskDelegate::drawTasks(QPainter* p, const QStyleOptionViewItem & option, const QModelIndex & index ) const {
    p->save();
    int margin = 15;
    int textSize = 30;
    int cols = (option.rect.width() / (textSize + margin));
    int dayWidth = textSize + margin;
//    int dayWidth = option.rect.width() / _totalDays;

    TaskModel* model = (TaskModel*)index.model();

    Task* task = model->task(index);
    Project* project = model->project(index);

    if (task != NULL) {
        vector<Task*>* subTasks = project->subTasks(task->id());
        if (subTasks->size() == 0) {
            drawTaskBar(p, task->startDate(), task->endDate(), dayWidth, option);
        } else {
            drawGroupBar(p, task->startDate(), task->endDate(), dayWidth, option);
        }
    } else {
        // If the project doesn't have tasks the dates will be NULL
        if (project->startDate() && project->endDate()) {
            drawGroupBar(p, project->startDate(), project->endDate(), dayWidth, option);
        }
    }
    p->restore();
}
