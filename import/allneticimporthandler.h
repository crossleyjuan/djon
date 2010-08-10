#ifndef ALLNETICIMPORTHANDLER_H
#define ALLNETICIMPORTHANDLER_H

#include <QXmlDefaultHandler>
#include <QXmlAttributes>
#include <QString>
#include <vector>

class Project;
class Task;
class TaskLog;
class DateTime;
class Template;

using namespace std;

class AllneticImportHandler : public QXmlDefaultHandler
{
public:
    AllneticImportHandler(Template* defaultTemplate, string* defaultStatus);
public:
    bool startDocument();
    bool startElement(const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& atts);
    bool endElement(const QString& namespaceURI, const QString& localName, const QString& qName);

    vector<Project*>* projects();

private:
    Project* parseProject(QXmlAttributes attrs);
    Task* parseTask(QXmlAttributes attrs);
    TaskLog* parseTaskLog(QXmlAttributes attrs);
    DateTime* parseDateTime(QString dte);

    vector<Project*>* _projects;

    Project* _currentProject;
    Task* _currentTask;
    Template* _defaultTemplate;
    string* _defaultStatus;
};

#endif // ALLNETICIMPORTHANDLER_H
