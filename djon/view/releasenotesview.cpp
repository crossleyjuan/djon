#include "releasenotesview.h"
#include "config.h"
#include "ui_releasenotesview.h"
#include <sstream>
#include <string>

ReleaseNotesView::ReleaseNotesView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReleaseNotesView)
{
    ui->setupUi(this);

    std::stringstream buf;
    buf << "http://d-jon.com/downloads/releasenotes.php?version=";
    buf << VERSION;
    ui->webView->setUrl(QUrl(buf.str().c_str()));
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
