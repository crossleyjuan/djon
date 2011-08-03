#include "fileutil.h"

#include <QtCore>
#include <QFile>
#include "data.h"


FileUtil::FileUtil()
{
}

void FileUtil::addMapping(Map map) {
    qDebug("FileUtil::addMapping()");
    QStringList fileContent;
    QFile file("mapping.txt");

    file.open(QIODevice::ReadOnly);
    QString mapLine;
    qDebug("FileUtil::addMapping task id: %s", map.task.taskId.toStdString().c_str());
    mapLine.append(map.task.taskId);
    mapLine.append(" ");
    mapLine.append(map.coreProject.id);
    mapLine.append(" ");
    mapLine.append(map.coreTask.id);
    mapLine.append("\n");

    qDebug("FileUtil::addMapping line: %s", mapLine.toStdString().c_str());
    bool replaced = false;
    while (!file.atEnd()) {
        QString currentLine(file.readLine());
        if (currentLine.startsWith(map.task.taskId)) {
            qDebug("FileUtil::addMapping map found, replacing: %s with %s", currentLine.toStdString().c_str(), mapLine.toStdString().c_str());
            currentLine = mapLine;
            replaced = true;
        }
        fileContent << currentLine;
    }
    file.close();
    if (!replaced) {
        qDebug("FileUtil::addMapping map found, adding line to the end of the file");
        fileContent << mapLine;
    }

    file.open(QIODevice::Truncate | QIODevice::WriteOnly);
    QString l;
    qDebug("FileUtil::addMapping saving contents to file");
    foreach (l, fileContent) {
        file.write(l.toStdString().c_str());
    }
    file.close();
    qDebug("FileUtil::addMapping file.close()");
    qDebug("out FileUtil::addMapping()");
}

Map FileUtil::readMapping(Task* task, QList<CoreProject> projects, QList<CoreTask> tasks) {
    qDebug("FileUtil::readMapping");
    QFile file("mapping.txt");

    file.open(QIODevice::ReadOnly);

    Map result;
    result.task.projectId = QString(task->project()->id()->c_str());
    result.task.taskId = QString(task->id()->c_str());
    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        QString currentLine(line);
        qDebug("FileUtil::readMapping currentLine: %s", currentLine.toStdString().c_str());
        if (currentLine.startsWith(QString(task->id()->c_str()))) {
            qDebug("FileUtil::readMapping task id: %s found starting map", task->id()->c_str());
            currentLine = currentLine.trimmed();
            currentLine = currentLine.remove(QChar('\n'));
            currentLine = currentLine.remove(QChar('\r'));
            QStringList vals = currentLine.split(QChar(' '));
            qDebug("FileUtil::readMapping 0: %s 1: %s 2: %s", vals.at(0).toStdString().c_str(), vals.at(1).toStdString().c_str(), vals.at(2).toStdString().c_str());
            CoreProject project;
            foreach (project, projects) {
                if (project.id == vals.at(1)) {
                    qDebug("FileUtil::readMapping project id: %s match", project.id.toStdString().c_str());
                    result.coreProject = project;
                    break;
                }
            }
            CoreTask coreTask;
            qDebug("FileUtil::readMapping looking for task: %s", vals.at(2).toStdString().c_str());
            foreach (coreTask, tasks) {
                if (coreTask.id.compare(vals.at(2)) == 0) {
                    qDebug("FileUtil::readMapping task id: %s match", coreTask.id.toStdString().c_str());
                    result.coreTask = coreTask;
                    break;
                }
            }

            break;
        }
    }
    file.close();
    qDebug("out FileUtil::readMapping");
    return result;
}

PluginSettings FileUtil::readSettings() {
    QFile file(QString("settings.dat"));
    return PluginSettings();
}
