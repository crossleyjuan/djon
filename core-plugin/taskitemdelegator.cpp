#include "taskitemdelegator.h"
#include <QComboBox>

TaskItemDelegator::TaskItemDelegator(QList<CoreTask> lst, QObject *parent) :
    QItemDelegate(parent)
{
    this->lstData = lst;
}

QWidget* TaskItemDelegator::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    if (!index.isValid()) {
        return new QWidget(parent);
    }
    QComboBox* comboBox = new QComboBox(parent);
    QStringList lst;
    CoreTask task;
    foreach (task, lstData) {
        lst << task.name;
    }

    comboBox->addItems(lst);
    return comboBox;
}


void TaskItemDelegator::setEditorData(QWidget *editor,
                                     const QModelIndex &index) const
 {
     int value = index.model()->data(index, Qt::UserRole).toInt();
     QComboBox *combo = static_cast<QComboBox*>(editor);
     combo->setCurrentIndex(value);
 }

 void TaskItemDelegator::setModelData(QWidget *editor, QAbstractItemModel *model,
                                    const QModelIndex &index) const
 {
     QComboBox *combo = static_cast<QComboBox*>(editor);

     model->setData(index, combo->currentIndex(), Qt::UserRole);
     model->setData(index, combo->currentText(), Qt::DisplayRole);
 }

 void TaskItemDelegator::updateEditorGeometry(QWidget *editor,
     const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
 {
     editor->setGeometry(option.rect);
 }
