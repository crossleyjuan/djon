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
