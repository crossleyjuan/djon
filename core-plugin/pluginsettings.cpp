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
