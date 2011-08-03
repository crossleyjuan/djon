#ifndef ITEMDELEGATOR_H
#define ITEMDELEGATOR_H

#include <QItemDelegate>
#include <QComboBox>
#include "datamanager.h"

class ItemDelegator : public QItemDelegate
{
    Q_OBJECT
public:
    explicit ItemDelegator(QList<CoreProject> lst, QObject *parent = 0);
    // editing
    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const;

signals:

public slots:

private:
    QList<CoreProject> lstData;
};

#endif // ITEMDELEGATOR_H
