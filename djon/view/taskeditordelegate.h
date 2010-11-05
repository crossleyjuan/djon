#ifndef TASKEDITORDELEGATE_H
#define TASKEDITORDELEGATE_H

#include <QItemDelegate>

class TaskEditorDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit TaskEditorDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const;

signals:

public slots:

};

#endif // TASKEDITORDELEGATE_H
