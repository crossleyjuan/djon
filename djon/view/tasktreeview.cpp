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

#include "tasktreeview.h"

#include <QPainter>

TaskTreeView::TaskTreeView(QWidget *parent) :
    QTreeView(parent)
{
}


void TaskTreeView::drawBranches(QPainter *painter,
                          const QRect &rect,
                          const QModelIndex &index) const {
//    if (model()->rowCount(index) > 0) {
//        int posCol = columnViewportPosition(2);
//        QRect r = rect;
//        r.setLeft(posCol);
//        QImage img(":/img/djon.png");
//        img.scaledToHeight(rect.height());
//        img.scaledToWidth(rect.width());
//        //painter->drawImage(r, img);
        QTreeView::drawBranches(painter, rect, index);
//    }
}

void TaskTreeView::drawRow(QPainter *painter,
                     const QStyleOptionViewItem &options,
                     const QModelIndex &index) const {
    QTreeView::drawRow(painter, options, index);
//    if (model()->rowCount(index) > 0) {
//        int posCol = columnViewportPosition(2);
//        QRect r = options.rect;
//        r.setLeft(posCol);
//        QImage img(":/img/djon.png");
//        img.scaledToHeight(10);
//        img.scaledToWidth(10);
//        r.setHeight(10);
//        r.setWidth(10);
//        painter->drawImage(r, img);
//    }
}

void TaskTreeView::dataIndexChanged(QModelIndex index) {
    update(index);
}
