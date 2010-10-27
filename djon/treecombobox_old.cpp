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
