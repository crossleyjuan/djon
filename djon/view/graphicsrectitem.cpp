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
