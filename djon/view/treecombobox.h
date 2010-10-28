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

    Q_PROPERTY(bool readOnly READ isReadOnly WRITE setReadOnly)

public:
    explicit TreeComboBox(QWidget *parent = 0);
    ~TreeComboBox();
    void setModel(const TaskModel* model);
    const TaskModel* model() const;
    QModelIndex currentModelIndex();
    void setReadOnly(bool);
    bool isReadOnly();

public slots:
    void setCurrentModelIndex(const QModelIndex& index);

protected:
    bool eventFilter(QObject *obj, QEvent *event);
private:
    void initialize();
//    void populate(const QModelIndex& index);

    Ui::TreeComboBox *ui;
    QTreeView* _treeView;
    const TaskModel* _model;
    std::map<std::string, int> _indexes;
    QWidget* _parent;
    QModelIndex _currentIndex;
    bool _readOnly;

private slots:
    void on_toolButton_clicked();
};

#endif // TREECOMBOBOX_H
