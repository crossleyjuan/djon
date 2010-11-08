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
    connect(_treeView, SIGNAL(clicked(QModelIndex)), this, SLOT(clicked(QModelIndex)));
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
    _treeView->setColumnHidden(1, true);
    _treeView->setColumnHidden(2, true);
    _treeView->setColumnHidden(3, true);
    _treeView->setColumnHidden(4, true);
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
    } else {
        ui->lineEdit->setText("");
    }
    if (_treeView->isVisible()) {
        _treeView->hide();
    }
}

void TreeComboBox::on_toolButton_clicked()
{
//    QRect parentRect;

    QRect frmGeometry = frameGeometry();
    QWidget* parentWindow = this;
    QPoint pos = this->pos();
    pos = this->parentWidget()->mapToGlobal(pos);
    int posx = pos.x();
    int posy = pos.y();
    QRect rect;
    rect.setX(posx);
    posy = posy + height();
    if ((posy+200) > qApp->desktop()->height()) {
        posy = posy - height() - 200;
    }
    rect.setY(posy);
    rect.setHeight(200);
    rect.setWidth(ui->lineEdit->width());

    _treeView->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::X11BypassWindowManagerHint);
    _treeView->setGeometry(rect);
    _treeView->show();
    _treeView->activateWindow();
    _treeView->setWindowTitle(tr(""));
    _treeView->setFocus(Qt::PopupFocusReason);
}

bool TreeComboBox::eventFilter(QObject *obj, QEvent *event) {
    if (obj == _treeView) {
        if (event->type() == QEvent::FocusOut) {
            _treeView->hide();
        }
    }
}

bool TreeComboBox::isReadOnly() {
    return _readOnly;
}

void TreeComboBox::setReadOnly(bool readOnly) {
    _readOnly = readOnly;
    ui->lineEdit->setReadOnly(readOnly);
    ui->toolButton->setEnabled(!readOnly);
}

void TreeComboBox::clicked(const QModelIndex &index) {
    setCurrentModelIndex(index);
    emit currentIndexChanged(index);
}

