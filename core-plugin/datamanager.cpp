#include "datamanager.h"
#include "fileutil.h"
#include <QtSql>
#include <stdlib.h>
#include <windows.h>
#include <lmcons.h>
#include <sstream>

DataManager::DataManager()
{
    _dataBase = NULL;
}


DataManager::~DataManager() {
    if (_dataBase) {
        if (_dataBase->isOpen()) {
            _dataBase->close();
        }
        delete(_dataBase);
    }
}

bool DataManager::connect() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setHostName("localhost");
    db.setDatabaseName("DBCore");
    db.setUserName("sa");
    db.setPassword("BizAgi2010");
    bool ok = db.open();
    if (!ok) {
        QSqlError error = db.lastError();
        qDebug(error.text().toStdString().c_str());
    }
    return ok;
}

QList<CoreProject> DataManager::projects() {
    QSqlQuery* query  = new QSqlQuery(QString("select Id, Code, name from dbo.Project  where Visible = 1 and IsTopLevel = 1 order by DisplayOrder"));
    QList<CoreProject> result;
    int index = 0;
    while (query->next()) {
        CoreProject project;
        project.id   = query->value(0).toString();
        project.code = query->value(1).toString();
        project.name = query->value(2).toString();
        project.index = index;
        index++;
        result << project;
    }
    _projects = result;
    return _projects;
}

QList<CoreTask> DataManager::tasks() {
    QSqlQuery* query  = new QSqlQuery(QString("select Id, Code, Name, IsTopLevel from dbo.Task where Visible = 1 order by Code"));
    QList<CoreTask> result;
    int index = 0;
    while (query->next()) {
        CoreTask task;
        task.id         = query->value(0).toString();
        task.code       = query->value(1).toString();
        task.name       = query->value(2).toString();
        task.isTopLevel = query->value(3).toBool();
        task.index = index;
        index++;
        result << task;
    }
    _tasks = result;
    return result;
}

Map DataManager::map(Task* task) {
    FileUtil util;
    Map result = util.readMapping(task, _projects, _tasks);
    return result;
}

bool DataManager::saveMap(Map map, DTime time) {
    FileUtil util;
    util.addMapping(map);

    bool res = saveDatabaseTimeRecord(map, time);
    return res;
}

int period(DTime time) {
    QDateTime currentTime = QDateTime::currentDateTime();

    int week = currentTime.date().weekNumber();
    int year = currentTime.date().year();

    std::string sql = format("select id from dbo.Period where YEAR = %d and WeekNumber = %d", year, week);
    QSqlQuery query(QString(sql.c_str()));
    int period = 0;
    if (query.next()) {
        period = query.value(0).toInt();
    } else {
        qDebug("Period year: %d week: %d not found", year, week);
        return -1;
    }
    return period;
}

bool DataManager::saveDatabaseTimeRecord(Map map, DTime time) {
    qDebug("DataManager::saveDatabaseTimeRecord");
    QString userName;

#if defined(UNICODE)
    if ( QSysInfo::WindowsVersion & QSysInfo::WV_NT_based)
    {
        TCHAR winUserName[UNLEN + 1]; // UNLEN is defined in LMCONS.H
        DWORD winUserNameSize = sizeof(winUserName);
        GetUserName( winUserName, &winUserNameSize );
        userName = QString::fromWCharArray( winUserName );
    } else
#endif
    {
        char winUserName[UNLEN + 1]; // UNLEN is defined in LMCONS.H
        DWORD winUserNameSize = sizeof(winUserName);
        GetUserNameA( winUserName, &winUserNameSize );
        userName = QString::fromLocal8Bit( winUserName );
    }

    qDebug("DataManager::saveDatabaseTimeRecord windows user: %s", userName.toStdString().c_str());
    QString userGuid;
    std::string userGuidSql = "select id from dbo.CoreUser where AccountName = 'VIS_BOG_HQ\\" + userName.toStdString() + "'";
    QSqlQuery sqlUserGuid(QString(userGuidSql.c_str()));
    if (sqlUserGuid.next()) {
        userGuid = sqlUserGuid.value(0).toString();
        qDebug("DataManager::saveDatabaseTimeRecord user guid: %s", userGuid.toStdString().c_str());
    } else {
        qDebug("User %s not found", userName.toStdString().c_str());
        return false;
    }

    int periodId = period(time);
    qDebug("DataManager::saveDatabaseTimeRecord period id: %d", periodId);
    if (periodId == -1) {
        return false;
    }
    QDate today = QDate::currentDate();
    std::stringstream ssSql;
    ssSql << "INSERT INTO TimeEntry (PeriodId,ProjectId,TaskId,CoreUserId,Date,Hours,Description,UpdatedOn,DisplayOrder,Version)";
    ssSql << " VALUES (:PeriodId,:ProjectId,:TaskId,:CoreUserId,:Date,:Hours,:Description,:UpdatedOn,:DisplayOrder,:Version)";
    QSqlQuery insert;
    insert.prepare(QString(ssSql.str().c_str()));
    insert.bindValue(":PeriodId", periodId);
    insert.bindValue(":ProjectId", map.coreProject.id);
    insert.bindValue(":TaskId", map.coreTask.id);
    insert.bindValue(":CoreUserId", userGuid);
    insert.bindValue(":Date", QDateTime::currentDateTime());
    double hours = (double)time.totalMinutes() / (double)60;
    insert.bindValue(":Hours", hours);
    insert.bindValue(":Description", "");
    insert.bindValue(":UpdatedOn", QDateTime::currentDateTime());
    insert.bindValue(":DisplayOrder", 0);
    insert.bindValue(":Version", 1);
    bool updated = insert.exec();
    if (!updated) {
        qDebug("An error has ocurred executing the insertion. Error: %s", insert.lastError().text().toStdString().c_str());
    }

    qDebug("out DataManager::saveDatabaseTimeRecord");
    return updated;
}
