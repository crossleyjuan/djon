#include "treecombobox.h"
#include "ui_treecombobox.h"

#include "data.h"
#include "TaskModel.h"
#include <QDesktopWidget>

TreeComboBox::TreeComboBox(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TreeComboBox)
{
    ui->setupUi(this);
    _treeView = new QTreeView(this);
    _treeView->setHeaderHidden(true);
    _treeView->setVisible(false);
    _treeView->installEventFilter(this);
    connect(_treeView, SIGNAL(clicked(QModelIndex)), this, SLOT(setCurrentModelIndex(QModelIndex)));
    _parent = parent;
}

TreeComboBox::~TreeComboBox()
{
    delete ui;
}

void TreeComboBox::setModel(const TaskModel* model) {
    _model = model;
    initialize();
}

const TaskModel* TreeComboBox::model() const {
    return _model;
}

//void TreeComboBox::populate(const QModelIndex &index) {
//    QVariant data = _model->data(index, Qt::DisplayRole);
//    ui->listWidget->addItem(data.toString());
//    for (int x = 0; x < _model->rowCount(index); x++) {
//        QModelIndex child = _model->index(x, 0, index);
//        populate(child);
//    }
//}

void TreeComboBox::initialize() {
    _treeView->setModel(const_cast<TaskModel*>(_model));
    setCurrentModelIndex(QModelIndex());
//    populate(root);
}

QModelIndex TreeComboBox::currentModelIndex() {
    return _currentIndex;
}

void TreeComboBox::setCurrentModelIndex(const QModelIndex& index) {
    _currentIndex = index;

    QVariant data = _model->data(index, Qt::DisplayRole);
    if (data.isValid()) {
        ui->lineEdit->setText(data.toString());
    }
    if (_treeView->isVisible()) {
        _treeView->hide();
    }
}

void TreeComboBox::on_toolButton_clicked()
{
    QRect parentRect;

    QRect frmGeometry = frameGeometry();
    QWidget* parentWindow = this;
    QPoint pos = this->pos();
    while ((parentWindow != NULL) && (!(parentWindow->windowFlags() & Qt::Window))) {
        parentWindow = (QWidget*)parentWindow->parentWidget();
    }
    parentRect = parentWindow->geometry();
    _treeView->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    QRect rect;
    rect.setX(parentRect.x() + frmGeometry.x() + parentWidget()->x());
    int y = parentRect.y() + frmGeometry.height() + parentWidget()->y();
    if ((y+200) > qApp->desktop()->height()) {
        y = parentRect.y() - 200;
    }
    rect.setY(y);
    rect.setHeight(200);
    rect.setWidth(ui->lineEdit->width());
    _treeView->setGeometry(rect);
    _treeView->show();
    _treeView->setFocus();
}

bool TreeComboBox::eventFilter(QObject *obj, QEvent *event) {
    if (obj == _treeView) {
        if (event->type() == QEvent::FocusOut) {
            _treeView->hide();
        }
    }
}

