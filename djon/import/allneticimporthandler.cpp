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

#include "allneticimporthandler.h"
#include "data.h"
#include "util.h"
#include <QtGui>
#include <string>


AllneticImportHandler::AllneticImportHandler(Template* defaultTemplate, string* defaultStatus)
{
    _defaultTemplate = defaultTemplate;
    _defaultStatus = defaultStatus;
    _projects = new vector<Project*>();
    _currentProject = NULL;
    _currentTask = NULL;
}

bool AllneticImportHandler::startDocument() {
    return true;
}

Project* AllneticImportHandler::parseProject(QXmlAttributes attrs) {
    Project* project = new Project();
    QString name = attrs.value(QString("name"));
    project->setName(new string(name.toStdString()));
    return project;
}

Task* AllneticImportHandler::parseTask(QXmlAttributes attrs) {
    Task* tsk = new Task(_currentProject);
    string* name = new string(attrs.value(QString("name")).toStdString());
    tsk->setShortDescription(name);

    tsk->setStartDate(parseDateTime(attrs.value("start")));
    tsk->setEndDate(parseDateTime(attrs.value("finish")));
    tsk->setDuration(Duration(1, 0, 0));
    tsk->setId(new string(attrs.value(QString("number")).toStdString()));
    tsk->setTemplateName(_defaultTemplate->name());
    tsk->setStatus(_defaultStatus);

    return tsk;
}

TaskLog* AllneticImportHandler::parseTaskLog(QXmlAttributes attrs) {
    TaskLog* log = new TaskLog();
    log->id = uuid();
    log->start = parseDateTime(attrs.value(QString("start")));
    log->end = parseDateTime(attrs.value(QString("finish")));

    return log;
}

DateTime* AllneticImportHandler::parseDateTime(QString dte) {
    QDateTime qdate = QDateTime::currentDateTime();
    if (dte.trimmed().length() != 0) {
        qdate = QDateTime::fromString(dte, QString("M/d/yyyy',' h:m:s AP"));
    }

    DateTime* dateTime = new DateTime(qdate);

    return dateTime;
}

bool AllneticImportHandler::startElement(const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& atts) {
    if (qName.compare(QString("project")) == 0) {
        _currentProject = parseProject(atts);
        _projects->push_back(_currentProject);
    }
    if (qName.compare(QString("task")) == 0) {
        _currentTask = parseTask(atts);
        _currentProject->addTask(_currentTask);
        if (errorOcurred()) {
            return false;
        }
    }
    if (qName.compare(QString("period")) == 0) {
        TaskLog* log = parseTaskLog(atts);
        _currentTask->addLog(log);
    }
    return true;
}

bool AllneticImportHandler::endElement(const QString& namespaceURI, const QString& localName, const QString& qName) {
    return true;
}

vector<Project*>* AllneticImportHandler::projects() {
    return _projects;
}
