#include <QtGui>
#include "exportdialog.h"
#include "ui_exportdialog.h"
#include "exportutility.h"
#include "data.h"

ExportDialog::ExportDialog(std::vector<Project*> projects, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportDialog)
{
    ui->setupUi(this);
    _projects = projects;
    _minDate = NULL;
    _maxDate = NULL;
    for (std::vector<Project*>::iterator iter = _projects.begin(); iter != _projects.end(); iter++) {
        Project* proj = *iter;
        DateTime* start = proj->startDate();
        DateTime* end = proj->endDate();
        if ((_minDate == NULL) || ((start != NULL) && (*start < *_minDate))) {
            _minDate = start;
        }
        if ((_maxDate == NULL) || ((end != NULL) && (*end < *_maxDate))) {
            _maxDate = end;
        }
    }
    if (_minDate != NULL) {
        ui->dteFrom->setDateTime(_minDate->toQDateTime());
    }
    if (_maxDate != NULL) {
        ui->dteTo->setDateTime(_maxDate->toQDateTime());
    }
    connect(this, SIGNAL(accepted()), this, SLOT(on_ExportDialog_accepted()));
}

ExportDialog::~ExportDialog()
{
    delete ui;
}

void ExportDialog::changeEvent(QEvent *e)
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

void ExportDialog::on_pushButton_clicked()
{
    QString selectedFileName = QFileDialog::getSaveFileName(this, tr("Export Project As"), tr(""), tr("XML Files (*.xml)"));
    if (selectedFileName.size() > 0){
        ui->fileName->setText(selectedFileName);
    }
}

void ExportDialog::done(int res) {
    bool valid = true;

    if (res == 1) {
        if (ui->fileName->text().length() == 0) {
            QMessageBox box(this);
            box.setText(tr("You should select a destination file name"));
            box.setWindowTitle(tr("d-jon export"));
            box.exec();
            valid = false;
        }
    }
    if (valid) {
        QDialog::done(res);
    }
}

void ExportDialog::on_ExportDialog_accepted()
{
    ExportUtility utility(_projects);
    if (ui->cbFilter->isChecked()) {
        DateTime* from = new DateTime(ui->dteFrom->dateTime());
        DateTime* to = new DateTime(ui->dteTo->dateTime());
        utility.executeExport(ui->fileName->text().toStdString(), from, to);
    } else {
        utility.executeExport(ui->fileName->text().toStdString());
    }
}


void ExportDialog::on_cbFilter_toggled(bool checked)
{
    ui->dteFrom->setEnabled(checked);
    ui->dteTo->setEnabled(checked);
}
