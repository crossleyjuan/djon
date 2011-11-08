// *********************************************************************************************************************
// file:
// author: Juan Pablo Crossley (crossleyjuan@gmail.com)
// created:
// updated:
// license:
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

#include "pluginsettings.h"
#include "ui_pluginsettings.h"

#include "fileutil.h"
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <stdio.h>

PluginSettings::PluginSettings(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::PluginSettings)
{
    ui->setupUi(this);
    populateDSN();
    _settings = FileUtil().readSettings();
}

PluginSettings::~PluginSettings()
{
    delete ui;
}


std::vector<std::string*>* PluginSettings::getDataSources() {
    SQLHENV env;
    SQLWCHAR dsn[256];
    SQLWCHAR desc[256];
    SQLSMALLINT dsn_ret;
    SQLSMALLINT desc_ret;
    SQLUSMALLINT direction;
    SQLRETURN ret;

    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
    SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (void *) SQL_OV_ODBC3, 0);

    direction = SQL_FETCH_FIRST;
    std::vector<std::string*>* result = new std::vector<std::string*>();
    while(SQL_SUCCEEDED(ret = SQLDataSources(env, direction,
                                             dsn, sizeof(dsn), &dsn_ret,
                                             desc, sizeof(desc), &desc_ret))) {
        std::wstring wdsn(dsn);
        std::string *s = new std::string(wdsn.begin(), wdsn.end());
        result->push_back(s);

        direction = SQL_FETCH_NEXT;
    }

    return result;
}

void PluginSettings::on_buttonBox_accepted()
{
}

void PluginSettings::populateDSN() {
    std::vector<std::string*>* dsns = getDataSources();
    for (std::vector<std::string*>::iterator i = dsns->begin(); i != dsns->end(); i++) {
        std::string* dsn = *i;
        ui->cboDSN->addItem(QString(dsn->c_str()));
    }
    delete(dsns);
}
