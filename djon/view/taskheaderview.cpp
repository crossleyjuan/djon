// *********************************************************************************************************************
// file:
// author: Juan Pablo Crossley (crossleyjuan@gmail.com)
// created:
// updated:
// license:
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

#include "taskheaderview.h"
#include "data.h"

TaskHeaderView::TaskHeaderView(std::vector<Project*>* projects, Qt::Orientation orientation, QWidget *parent) :
    QHeaderView(orientation, parent)
{
//    setDefaultSectionSize(200);
    setProjects(projects);
}

void TaskHeaderView::paintSection ( QPainter * painter, const QRect & rect, int logicalIndex ) const {
    QPainter* p = painter;
//    p->save();

    p->setBrush(QBrush(QColor("white")));
    p->setPen(QPen(QColor("white")));
    p->drawRect(0, 0, width(), height());

    if (!_startDate || !_endDate) {
        return;
    }
    int textSize = 30;
    int margin = 15;
    int cols = (rect.width() / (textSize + margin));
    int columnSize = textSize + margin;
    const int HEADER_HEIGHT = 21;

    QDateTime startDate = _startDate->toQDateTime();
    int days = 1;//startDate.daysTo(*_endDate->toQDateTime()) / cols;

    p->setFont(QFont("Arial", 9));
    int offset = (horizontalOffset() / columnSize);
    startDate = startDate.addDays(offset);
    for (int x = 0; x < (cols + 1); x++) {
        QColor barcolor;
        if ((x % 2) > 0) {
            barcolor = QColor("white");
        } else {
            barcolor = QColor(235, 235, 235);
        }
        p->setBrush(QBrush(barcolor));
        p->setPen(QPen(barcolor));
        p->drawRect(x*columnSize, HEADER_HEIGHT, (x+1)*columnSize, this->rect().height());

        QPen pen(QColor(200, 200, 200));
        pen.setStyle(Qt::DashLine);
        p->setPen(pen);
        p->drawLine(x*columnSize, HEADER_HEIGHT, x*columnSize, this->rect().height());

        p->setPen(QColor("black"));
        p->drawText(x*(textSize + margin), 15, startDate.toString("dd-MMM"));
        startDate = startDate.addDays(days);
    }
}

void TaskHeaderView::refresh() {
    DateTime* startDate = NULL;
    DateTime* endDate = NULL;

    for (std::vector<Project*>::iterator itProj = _projects->begin(); itProj != _projects->end(); itProj++) {
        Project* proj = *itProj;
        std::vector<Task*>* tasks = proj->tasks();
        for (std::vector<Task*>::iterator itTask = tasks->begin(); itTask != tasks->end(); itTask++) {
            Task* tsk = *itTask;
            DateTime* tskStartDate = tsk->startDate();
            if ((startDate == NULL) || (*startDate > *tskStartDate)) {
                startDate = tskStartDate;
            }
            DateTime* tskEndDate = tsk->endDate();
            if ((endDate == NULL) || (*endDate < *tskEndDate)) {
                endDate = tskEndDate;
            }
        }
        delete(tasks);
    }
//    if (startDate != NULL) {
//        totalDays = startDate->daysTo(*endDate) + 1;
//        if ((totalDays > 1) && (totalDays < 8)) {
//            scale = DAY;
//        } else if ((totalDays > 7) && (totalDays < 16)) {
//            scale = HALF_MONTH;
//        } else if (totalDays > 15) {
//            scale = MONTH;
//        }
//    }
    _startDate = startDate;
    _endDate = endDate;
    if (_startDate && _endDate) {
        int days = startDate->toQDateTime().daysTo(endDate->toQDateTime());
        setDefaultSectionSize( (days+2) * 45);
    }
}

void TaskHeaderView::setProjects(std::vector<Project *> *projects) {
    _projects = projects;
    refresh();
}
