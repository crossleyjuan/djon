#ifndef TREECOMBOBOX_H
#define TREECOMBOBOX_H

#include <QWidget>
#include <QTreeView>
#include <vector>
#include <map>
#include <string>

class Project;
class TaskModel;

namespace Ui {
    class TreeComboBox;
}

class TreeComboBox : public QWidget
{
    Q_OBJECT

public:
    explicit TreeComboBox(const TaskModel* model, QWidget *parent = 0);
    ~TreeComboBox();
public slots:
    void setModel(const TaskModel* model);

private:
    void initialize();
    void populate(const QModelIndex& index);

    Ui::TreeComboBox *ui;
    QTreeView* _treeView;
    const TaskModel* _model;
    std::map<std::string, int> _indexes;
};

#endif // TREECOMBOBOX_H
