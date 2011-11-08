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
