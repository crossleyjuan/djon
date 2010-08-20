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
    ui->idleTimeOut->setValue(idleTimeout);
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
        int idleTimeOut = ui->idleTimeOut->value();
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
            QDialog::done(QDialog::Accepted);
        }
    } else {
        QDialog::done(res);
    }
}
