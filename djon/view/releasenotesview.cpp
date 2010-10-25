#include "releasenotesview.h"
#include "ui_releasenotesview.h"

ReleaseNotesView::ReleaseNotesView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReleaseNotesView)
{
    ui->setupUi(this);
}

ReleaseNotesView::~ReleaseNotesView()
{
    delete ui;
}

void ReleaseNotesView::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
