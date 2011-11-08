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

#include "tasklogdelegate.h"

#include <QtGui>
#include "data.h"

TaskLogDelegate::TaskLogDelegate(QObject *parent)
    :QItemDelegate(parent)
{
}

QWidget *TaskLogDelegate::createEditor(QWidget *parent,
                                       const QStyleOptionViewItem &option ,
                                       const QModelIndex &index) const
{
    switch (index.column()) {
    case 0:
        return new QLineEdit(parent);
    case 1:
    case 2:
        return new QDateTimeEdit(QDateTime::currentDateTime(), parent);
    }

    return new QWidget();
}

void TaskLogDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    TaskLog* log = (TaskLog*)index.internalPointer();

    switch (index.column()) {
    case 0: {
            QLineEdit* edit = static_cast<QLineEdit*>(editor);
            if (log != NULL) {
                if (log->logDescription != NULL) {
                    edit->setText(QString(log->logDescription->c_str()));
                }
            }
            break;
        }
    case 1: {
            QDateTimeEdit* dedit = static_cast<QDateTimeEdit*>(editor);
            if (log != NULL) {
                dedit->setDateTime(log->start->toQDateTime());
            }
            break;
        }
    case 2: {
            QDateTimeEdit* dedit = static_cast<QDateTimeEdit*>(editor);
            if (log != NULL) {
                dedit->setDateTime(log->end->toQDateTime());
            }
            break;
        }
    }
}

void TaskLogDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
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

void TaskLogDelegate::updateEditorGeometry(QWidget *editor,
                                           const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}
