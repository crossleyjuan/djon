#include "treecombobox.h"
#include "ui_treecombobox.h"

#include "data.h"
#include "TaskModel.h"

TreeComboBox::TreeComboBox(const TaskModel* taskModel, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TreeComboBox)
{
    ui->setupUi(this);
    _treeView = new QTreeView(this);
    setModel(taskModel);
}

TreeComboBox::~TreeComboBox()
{
    delete ui;
}

void TreeComboBox::setModel(const TaskModel* model) {
    _model = model;
    initialize();
}

void TreeComboBox::populate(const QModelIndex &index) {
    QVariant data = _model->data(index, Qt::DisplayRole);
    ui->listWidget->addItem(data.toString());
    for (int x = 0; x < _model->rowCount(index); x++) {
        QModelIndex child = _model->index(x, 0, index);
        populate(child);
    }
}

void TreeComboBox::initialize() {
    ui->listWidget->clear();
    QModelIndex root = _model->index(0, 0, QModelIndex());
    populate(root);
}
