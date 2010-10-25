#ifndef TREECOMBOBOX_H
#define TREECOMBOBOX_H

#include <QtGui>

class Task;

class TreeComboBox : public QComboBox {
    Q_OBJECT
public:
    TreeComboBox(QWidget *parent = 0);
    ~TreeComboBox();
    bool eventFilter(QObject* object, QEvent* event);
    virtual void showPopup();
    virtual void hidePopup();
    QModelIndex currentModelIndex();
    void setCurrentModelIndex(const QModelIndex& index);

protected:

private:
    bool skipNextHide;
};

#endif // TREECOMBOBOX_H
