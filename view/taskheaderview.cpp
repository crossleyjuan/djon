#include "taskheaderview.h"

TaskHeaderView::TaskHeaderView(Qt::Orientation orientation, QWidget *parent) :
    QHeaderView(orientation, parent)
{
}

void TaskHeaderView::paintSection ( QPainter * painter, const QRect & rect, int logicalIndex ) const {
    QRect secRect = rect;
    secRect.adjust(10, 0, 0, 0);
    switch (logicalIndex) {
    case 0:
        QHeaderView::paintSection(painter, rect, logicalIndex);
//        painter->drawText(secRect, QString("asdfs"));
        break;
    case 1:
        painter->drawText(secRect, QString("otro"));
        break;
    }

}
