#include "combotree.h"
#include "ui_combotree.h"

ComboTree::ComboTree(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ComboTree)
{
    ui->setupUi(this);
}

ComboTree::~ComboTree()
{
    delete ui;
}

void ComboTree::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
