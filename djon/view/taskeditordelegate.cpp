// *********************************************************************************************************************
// file:    taskeditordelegate.cpp
// author:  Juan Pablo Crossley (crossleyjuan@gmail.com)
// created: Oct/1/2010
// updated: Nov/7/2011
// license: GPL 2
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

#include "taskeditordelegate.h"

#include <QtGui>

TaskEditorDelegate::TaskEditorDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}


QWidget *TaskEditorDelegate::createEditor(QWidget *parent,
                                       const QStyleOptionViewItem &option ,
                                       const QModelIndex &index) const
{
    switch (index.column()) {
    case 0: // Description
        return new QLineEdit(parent);
    case 1:
        return new QCheckBox(parent);
    }

    return new QWidget();
}

void TaskEditorDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    QVariant data = index.data();

    switch (index.column()) {
    case 0: {
            QLineEdit* edit = static_cast<QLineEdit*>(editor);
            edit->setText(data.toString());
            break;
        }
    case 1: {
            QCheckBox* cedit = static_cast<QCheckBox*>(editor);
            cedit->setChecked(data.Bool);
            break;
        }
    }
}

void TaskEditorDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    QVariant data;
    switch (index.column()) {
    case 0: {
            QLineEdit* edit = static_cast<QLineEdit*>(editor);
            data = edit->text();
            break;
        }
    case 1:
    case 2: {
            QDateTimeEdit* dedit = static_cast<QDateTimeEdit*>(editor);
            data = dedit->dateTime();
            break;
        }
    }

    model->setData(index, data, Qt::EditRole);
}

void TaskEditorDelegate::updateEditorGeometry(QWidget *editor,
                                           const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}
