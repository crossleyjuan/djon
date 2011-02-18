#ifndef GRAPHICSRECTITEM_H
#define GRAPHICSRECTITEM_H

#include <QObject>
#include <QGraphicsRectItem>
#include <QModelIndex>

class GraphicsRectItem : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    GraphicsRectItem(const QRectF &rect, QModelIndex index, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
    QModelIndex index() const;
signals:
    void itemHoverEnter(QModelIndex index);
    void itemHoverLeave(QModelIndex index);

public slots:

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
//    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    virtual bool sceneEvent(QEvent * event);

private:
    QModelIndex _index;
};

#endif // GRAPHICSRECTITEM_H
