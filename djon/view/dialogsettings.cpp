// *********************************************************************************************************************
// file:    dialogsettings.cpp
// author:  Juan Pablo Crossley (crossleyjuan@gmail.com)
// created: Jul/29/2010
// updated: Nov/7/2011
// license: GPL 2
//
// This file is part of the djon project, all the source code is licensed under the terms of the GPL license,
// the application and libraries are provided as-is and free of use under the terms explained in the file COPYING.GPL
// the authors are not responsible for any problem that this program, or any of its libraries, may cause.
// Its authors create this application in order to make the world a better place to live, but you should use it on
// your own risks.
// 
// Also, be adviced that, the GPL license force the committers to ensure this application will be free of use, thus
// if you do any modification you will be required to provide it for free unless you use it for personal use (you may 
// charge yourself if you want), bare in mind that you will be required to provide a copy of the license terms that ensures
// this program will be open sourced and all its derivated work will be too.
// *********************************************************************************************************************

#include <QtGui>
#include "dialogsettings.h"
#include "ui_dialogsettings.h"
#include "util.h"
#include "data.h"
#include <sstream>

DialogSettings::DialogSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSettings)
{
    ui->setupUi(this);
    bool closeToSysTray = getSettings()->closeToTray();
    ui->cbCloseToSysTray->setChecked(closeToSysTray);

    long idleTimeout = getSettings()->idleTimeOut();
    DTime t(idleTimeout);
    QTime time = t.toQTime();
    ui->idleTimeOut->setTime(time);

    populateLogOrderBy();
    populateCheckUpdate();

    int updateCheck = getSettings()->checkUpdate();
    ui->cboUpdate->setCurrentIndex(ui->cboUpdate->findData(updateCheck));
    ui->transparency->setValue(getSettings()->transparency());

    /**
    loadCalendars();
    loadTemplates();
    **/
    ui->tab->removeTab(2);
    ui->tab->removeTab(1);
    ui->tab->setCurrentIndex(0);
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
        int idleTimeOut = timeOut.totalSecs();
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
            getSettings()->setIdleTimeOut(idleTimeOut);
            getSettings()->setCloseToTray(ui->cbCloseToSysTray->isChecked());
            getSettings()->setCheckUpdate(ui->cboUpdate->itemData(ui->cboUpdate->currentIndex()).toInt());
            getSettings()->setTransparency(ui->transparency->value());
            getSettings()->save();
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

    char* sortPref = readPreference("log-sort", "");
    string sort(sortPref);
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
    free(sortPref);
}

void DialogSettings::populateCheckUpdate() {
    ui->cboUpdate->addItem("every 4 hours", 240);
    ui->cboUpdate->addItem("every 8 hours", 480);
    ui->cboUpdate->addItem("every 24 hours", 1440);
    ui->cboUpdate->addItem("Never", 0);
}

void DialogSettings::loadCalendars() {
    map<string, Calendar*> cals = calendars();

    ui->calendarList->addItem("Default"); // Precreated calendar

    qDebug("Calendars size: %d", cals.size());
    for (map<string, Calendar*>::iterator iter = cals.begin(); iter != cals.end(); iter++) {
        ui->calendarList->addItem(QString(iter->first.c_str()));
    }
}

void DialogSettings::loadTemplates() {
    vector<Template*>* templates = readTemplates();
    QStringList list;
    for (vector<Template*>::iterator iter = templates->begin(); iter != templates->end(); iter++) {
        Template* tpl = *iter;
        QString description(tpl->description()->c_str());
        list.append(description);
    }
    ui->templatesList->addItems(list);
}
