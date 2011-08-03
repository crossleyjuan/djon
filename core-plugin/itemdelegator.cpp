#include "itemdelegator.h"
#include <QComboBox>

ItemDelegator::ItemDelegator(QList<CoreProject> lst, QObject *parent) :
    QItemDelegate(parent)
{
    this->lstData = lst;
}

QWidget* ItemDelegator::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    if (!index.isValid()) {
        return new QWidget(parent);
    }
    QComboBox* comboBox = new QComboBox(parent);
    QStringList lst;
    CoreProject project;
    foreach (project, lstData) {
        lst << project.name;
    }

    comboBox->addItems(lst);
    return comboBox;
}


void ItemDelegator::setEditorData(QWidget *editor,
                                     const QModelIndex &index) const
 {
     int value = index.model()->data(index, Qt::UserRole).toInt();
     QComboBox *combo = static_cast<QComboBox*>(editor);
     combo->setCurrentIndex(value);
 }

 void ItemDelegator::setModelData(QWidget *editor, QAbstractItemModel *model,
                                    const QModelIndex &index) const
 {
     QComboBox *combo = static_cast<QComboBox*>(editor);

     model->setData(index, combo->currentIndex(), Qt::UserRole);
     model->setData(index, combo->currentText(), Qt::DisplayRole);
 }

 void ItemDelegator::updateEditorGeometry(QWidget *editor,
     const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
 {
     editor->setGeometry(option.rect);
 }
