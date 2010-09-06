#include <QtGui>
#include "dialogsettings.h"
#include "ui_dialogsettings.h"
#include "util.h"
#include <sstream>

DialogSettings::DialogSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSettings)
{
    ui->setupUi(this);
    bool closeToSysTray = atoi(readConfValue("close-to-systray", "1"));
    ui->cbCloseToSysTray->setChecked(closeToSysTray);

    int idleTimeout = atoi(readConfValue("idle-timeout", "300"));
    DTime t(idleTimeout);
    QTime* time = t.toQTime();
    ui->idleTimeOut->setTime(*time);

    populateLogOrderBy();
}

DialogSettings::~DialogSettings()
{
    delete ui;
}

void DialogSettings::changeEvent(QEvent *e)
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

void DialogSettings::on_DialogSettings_accepted()
{
}

void DialogSettings::done(int res) {
    if (res == QDialog::Accepted) {
        DTime timeOut(ui->idleTimeOut->time());
        int idleTimeOut = timeOut.secs();
        bool save = true;
        if (idleTimeOut == 0) {
            QMessageBox box;
            box.setWindowTitle(tr("d-jon settings"));
            box.setText("The idle timeout 0 means you'll disable the away detection. Are you sure?");
            box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            int res = box.exec();
            if (res == QMessageBox::No) {
                save = false;
            }
        }
        if (save) {
            std::stringstream ss;
            ss << idleTimeOut;
            std::string sTimeOut = ss.str();
            writeConfValue("idle-timeout", sTimeOut.c_str());
            writeConfValue("close-to-systray", ui->cbCloseToSysTray->isChecked() ? "1": "0");
            std::string logSort = ui->logOrderBy->itemData(ui->logOrderBy->currentIndex()).toString().toStdString();
            writePreference("log-sort", logSort);
            QDialog::done(QDialog::Accepted);
        }
    } else {
        QDialog::done(res);
    }
}

void DialogSettings::populateLogOrderBy() {
    ui->logOrderBy->addItem(tr("Description ASC"), "0++0");
    ui->logOrderBy->addItem(tr("Description DES"), "0++1");
    ui->logOrderBy->addItem(tr("Start Date ASC"), "1++0");
    ui->logOrderBy->addItem(tr("Start Date DES"), "1++1");
    ui->logOrderBy->addItem(tr("End Date ASC"), "2++0");
    ui->logOrderBy->addItem(tr("End Date DES"), "2++1");

    string sort(readPreference("log-sort", ""));
    if (sort.length() > 0) {
        QString pref(sort.c_str());
        int rows = ui->logOrderBy->count();
        for (int index = 0; index < rows; index++) {
            QString data = ui->logOrderBy->itemData(index).toString();
            if (data.compare(pref) == 0) {
                ui->logOrderBy->setCurrentIndex(index);
                break;
            }
        }
    }
}
