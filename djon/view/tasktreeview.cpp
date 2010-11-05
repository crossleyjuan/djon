#include "tasktreeview.h"

#include <QPainter>

TaskTreeView::TaskTreeView(QWidget *parent) :
    QTreeView(parent)
{
}


void TaskTreeView::drawBranches(QPainter *painter,
                          const QRect &rect,
                          const QModelIndex &index) const {
    if (model()->rowCount(index) > 0) {
        int posCol = columnViewportPosition(2);
        QRect r = rect;
        r.setLeft(posCol);
        QImage img(":/img/djon.png");
        img.scaledToHeight(rect.height());
        img.scaledToWidth(rect.width());
        //painter->drawImage(r, img);
        QTreeView::drawBranches(painter, r, index);
    }
}

void TaskTreeView::drawRow(QPainter *painter,
                     const QStyleOptionViewItem &options,
                     const QModelIndex &index) const {
    QTreeView::drawRow(painter, options, index);
    if (model()->rowCount(index) > 0) {
        int posCol = columnViewportPosition(2);
        QRect r = options.rect;
        r.setLeft(posCol);
        QImage img(":/img/djon.png");
        img.scaledToHeight(10);
        img.scaledToWidth(10);
        r.setHeight(10);
        r.setWidth(10);
        painter->drawImage(r, img);
    }
}
