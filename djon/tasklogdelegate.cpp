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
                dedit->setDateTime(*log->start->toQDateTime());
            }
            break;
        }
    case 2: {
            QDateTimeEdit* dedit = static_cast<QDateTimeEdit*>(editor);
            if (log != NULL) {
                dedit->setDateTime(*log->end->toQDateTime());
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
