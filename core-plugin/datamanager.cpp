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
    db.setDatabaseName("DBCore");
    db.setUserName("djon");
    db.setPassword("dj0n2011");
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

bool DataManager::saveMap(Map map) {
    FileUtil util;
    util.addMapping(map);

    bool res = saveDatabaseTimeRecord(map);
    return res;
}

int period(QDateTime date) {

    int week = date.date().weekNumber() + 1;
    int year = date.date().year();

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

bool DataManager::saveDatabaseTimeRecord(Map map) {
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

    int periodId = period(map.date.toQDateTime());
    qDebug("DataManager::saveDatabaseTimeRecord period id: %d", periodId);
    if (periodId == -1) {
        return false;
    }

    int displayOrder = 1;
    QSqlQuery displayOrderSql;
    if (displayOrderSql.prepare("select Max(displayOrder) from dbo.TimeEntry where PeriodId = :Period and CoreUserId = :CoreUserId")) {
        displayOrderSql.bindValue(":Period", periodId);
        displayOrderSql.bindValue(":CoreUserId", userGuid);
        if (displayOrderSql.exec() && displayOrderSql.next()) {
            displayOrder = displayOrderSql.value(0).toInt() + 1;
        } else {
            displayOrder = 1;
        }
    } else {
        qDebug("Error getting the displayOrder");
        return false;
    }
    qDebug("DataManager::saveDatabaseTimeRecord displayOrder: %d", displayOrder);
    std::stringstream ssSql;
    ssSql << "INSERT INTO TimeEntry (PeriodId,ProjectId,TaskId,CoreUserId,Date,Hours,Description,UpdatedOn,DisplayOrder,Version)";
    ssSql << " VALUES (:PeriodId,:ProjectId,:TaskId,:CoreUserId,:Date,:Hours,:Description,:UpdatedOn,:DisplayOrder,:Version)";
    QSqlQuery insert;
    insert.prepare(QString(ssSql.str().c_str()));
    insert.bindValue(":PeriodId", periodId);
    insert.bindValue(":ProjectId", map.coreProject.id);
    insert.bindValue(":TaskId", map.coreTask.id);
    insert.bindValue(":CoreUserId", userGuid);
    insert.bindValue(":Date", map.date.toQDateTime());
    double minutes = ((double)((int)(map.time.totalMinutes() * 100))/(double)100);

    insert.bindValue(":Hours", minutes);
    insert.bindValue(":Description", map.description);
    insert.bindValue(":UpdatedOn", QDateTime::currentDateTime());
    insert.bindValue(":DisplayOrder", displayOrder);
    insert.bindValue(":Version", 2);
    bool updated = insert.exec();
    if (!updated) {
        qDebug("An error has ocurred executing the insertion. Error: %s", insert.lastError().text().toStdString().c_str());
    }

    qDebug("out DataManager::saveDatabaseTimeRecord");
    return updated;
}
