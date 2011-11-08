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

#include "graphicsrectitem.h"
#include <QGraphicsSceneHoverEvent>

GraphicsRectItem::GraphicsRectItem(const QRectF &rect, QModelIndex index, QGraphicsItem *parent, QGraphicsScene *scene) :
    QGraphicsRectItem(rect, parent, scene)
{
    _index = index;
}

void GraphicsRectItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    emit itemHoverEnter(_index);
    //QGraphicsRectItem::hoverEnterEvent(event);
//    update();
    event->setAccepted(false);
    qDebug("GraphicsRectItem::hoverEnterEvent");
}

//void GraphicsRectItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
//    qDebug("GraphicsRectItem::hoverLeaveEvent");
//    emit itemHoverLeave(_index);
//    QGraphicsRectItem::hoverLeaveEvent(event);
//}

QModelIndex GraphicsRectItem::index() const {
    return _index;
}

//bool GraphicsRectItem::eventFilter(QObject *obj, QEvent *evt) {
//    if (evt->type() == QEvent::HoverLeave) {
//        qDebug("Hover leave");
//    }
//    if (evt->type() == QEvent::HoverEnter) {
//        qDebug("Hover Enter");
//    }
//    return false;
//}

bool GraphicsRectItem::sceneEvent(QEvent *event) {
    if (event->type() == QEvent::GraphicsSceneHoverLeave) {
        emit itemHoverLeave(_index);
    }
    if (event->type() == QEvent::GraphicsSceneHoverEnter) {
        emit itemHoverEnter(_index);
    }
    return false;
}
