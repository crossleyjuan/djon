// *********************************************************************************************************************
// file:    timeview.cpp
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

#include "timeview.h"
#include "TaskModel.h"

TimeView::TimeView(QWidget *parent) :
    AbstractViewer(parent)
{
    QLayout* lay = new QVBoxLayout();
    _model = NULL;
    _timeScene = NULL;
    _headerScene = NULL;
    _headerView.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _headerView.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _todayPos = 0;
    connect(_timeView.horizontalScrollBar(), SIGNAL(valueChanged(int)), _headerView.horizontalScrollBar(), SLOT(setValue(int)));
    lay->addWidget(&_headerView);
    lay->addWidget(&_timeView);
    lay->setSpacing(0);
    lay->setContentsMargins(0, 0, 0, 0);
    setLayout(lay);
}

void TimeView::createHeader() {
    _headerScene->clear();

    QSize size = headerSizeHint();

    QLinearGradient grad(0, 0, 0, size.height());
    int dark = 210;
    grad.setColorAt(1, QColor(dark, dark, dark));
    grad.setColorAt(0.7, QColor(dark + 20, dark + 20, dark + 20));
    grad.setColorAt(0, Qt::white);
    QBrush background(grad);
    QPen pen(QColor(220, 220, 220));
    QSize sizeHint = _timeScene->viewSizeHint();
    _headerScene->addRect(0, 0, sizeHint.width() + verticalScrollBar()->width() + 200, size.height() + 1, pen, background)->setZValue(0);

    int textSize = 30;
    int margin = 15;
    int columnSize = 45;

    DateTime startDate = _timeScene->startDate().addDays(-1);
    DateTime today;
    today.setHour(0);
    today.setMin(0);
    today.setSecs(0);
    for (int x = 0; x < _timeScene->totalDays(); x++) {
        QPen textPen(Qt::black);
        QGraphicsSimpleTextItem* text = _headerScene->addSimpleText(startDate.addDays(1).toQDateTime().toString("dd-MMM"), QFont("Arial", 8));
        text->setPos(x * 45 + 2, 3);
        text->setVisible(true);
//        text->setPen(textPen);
        text->setZValue(1);
        if (startDate == today) {
            _todayPos = (x * 45 + 2);
        }
        startDate = startDate.addDays(1);
    }
    _headerView.setMaximumHeight(headerSizeHint().height());
}

QSize TimeView::headerSizeHint() {
    QVariant vSize = _model->headerData(0, Qt::Vertical, Qt::SizeHintRole);
    QSize sizeHint = QSize(10, 23);
    if (vSize.canConvert<QSize>()) {
        sizeHint = qvariant_cast<QSize>(vSize);
    }
    return sizeHint;
}


void TimeView::setModel(TaskModel *taskModel) {
    _model = taskModel;
    _timeScene = new TimeScene(this);
    connect(_timeScene, SIGNAL(itemHoverEnter(QModelIndex)), this, SLOT(receiveItemHoverEnter(QModelIndex)));
    connect(_timeScene, SIGNAL(itemHoverLeave(QModelIndex)), this, SLOT(receiveItemHoverLeave(QModelIndex)));
    _headerScene = new QGraphicsScene(this);
    _headerView.setScene(_headerScene);
    _headerView.setAlignment(Qt::AlignLeft | Qt::AlignTop);
    _timeView.setScene(_timeScene);
    _timeView.setAlignment(Qt::AlignLeft | Qt::AlignTop);

    _timeScene->setModel(_model);
    createHeader();
}

QScrollBar* TimeView::horizontalScrollBar() {
    return _timeView.horizontalScrollBar();
}

QScrollBar* TimeView::verticalScrollBar() {
    return _timeView.verticalScrollBar();
}

void TimeView::collapse(const QModelIndex& index) {
    _timeScene->collapse(index);
}

void TimeView::expand(const QModelIndex& index) {
    _timeScene->expand(index);
}

void TimeView::refresh() {
    _timeScene->refresh();
    createHeader();
}

void TimeView::scrollToday() {
    if (_todayPos > 0) {
        QPoint currentPos = this->_timeView.pos();
        currentPos.setX(_todayPos);
        this->_timeView.centerOn(currentPos);
    }
}

void TimeView::receiveItemHoverEnter(QModelIndex index) {
    emit itemHoverEnter(index);
}

void TimeView::receiveItemHoverLeave(QModelIndex index) {
    emit itemHoverLeave(index);
}

void TimeView::resizeEvent(QResizeEvent *evt) {
    if (_timeScene) {
        refresh();
    }
}
