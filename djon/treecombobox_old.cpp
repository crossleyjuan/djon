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

#include "treecombobox.h"

#include <QtGui>

TreeComboBox::TreeComboBox(QWidget *parent) :
    QComboBox(parent)
{
    QTreeView* v = new QTreeView(this);
    setView(v);
    this->setMaxVisibleItems(100);
    //v->header()->hide();
    v->viewport()->installEventFilter(this);
}

bool TreeComboBox::eventFilter(QObject* object, QEvent* event)
{
    if (event->type() == QEvent::MouseButtonPress && object == view()->viewport())
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        QModelIndex index = view()->indexAt(mouseEvent->pos());
        if (!view()->visualRect(index).contains(mouseEvent->pos()))
            skipNextHide = true;
    }
    return false;
}

void TreeComboBox::showPopup()
{
    setRootModelIndex(QModelIndex());
    QTreeView* v = (QTreeView*)view();
    v->expandAll();
    QComboBox::showPopup();
}

void TreeComboBox::hidePopup()
{
    setRootModelIndex(view()->currentIndex().parent());
    setCurrentModelIndex(view()->currentIndex());
    if (skipNextHide)
        skipNextHide = false;
    else
        QComboBox::hidePopup();
}


TreeComboBox::~TreeComboBox()
{
}

QModelIndex TreeComboBox::currentModelIndex() {
    return view()->currentIndex();
}

void TreeComboBox::setCurrentModelIndex(const QModelIndex& index) {
    QTreeView* tview = (QTreeView*)view();

    tview->selectionModel()->setCurrentIndex(index, QItemSelectionModel::SelectCurrent);

    QString str = view()->model()->data(index, Qt::DisplayRole).toString();
    qDebug("selected: %s", str.toStdString().c_str());
}
