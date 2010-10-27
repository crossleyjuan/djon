#ifndef COMBOTREE_H
#define COMBOTREE_H

#include <QWidget>

namespace Ui {
    class ComboTree;
}

class ComboTree : public QWidget {
    Q_OBJECT
public:
    ComboTree(QWidget *parent = 0);
    ~ComboTree();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ComboTree *ui;
};

#endif // COMBOTREE_H
