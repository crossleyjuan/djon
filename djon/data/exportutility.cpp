#include "exportutility.h"
#include "data.h"
#include <fstream>
#include <sstream>

ExportUtility::ExportUtility(vector<Project*> projects) {
    _projects = projects;
}

ExportUtility::~ExportUtility() {

}

ExportUtility::ExportUtility(const ExportUtility& utility) {
    _projects = utility._projects;
}

void ExportUtility::executeExport(std::string fileName, DateTime* logsFrom, DateTime* logsTo) {
    ofstream result(fileName.c_str());

    result << "<?xml version=\"1.0\" encoding=\"windows-1252\"?>\n";
    result << "<projects>\n";
    for (vector<Project*>::iterator iterProj = _projects.begin(); iterProj != _projects.end(); iterProj++) {
        Project* proj = *iterProj;
        result << "<project ";
        result << "name=\"" << *proj->name() << "\" ";
        result << "totalTime=\"" << proj->totalTime()->toChar() << "\" ";

        DTime totalProjectLogTime;
        stringstream ssTasks;
        ssTasks << "<tasks>\n";
        vector<Task*>* tasks = proj->tasks();
        bool includeTask = false;
        for (vector<Task*>::iterator iterTask = tasks->begin(); iterTask != tasks->end(); iterTask++) {
            Task* task = *iterTask;
            stringstream ssTask;
            ssTask << "<task ";
            ssTask << "number=\"" << *task->id() << "\" ";
            ssTask << "name=\"" << *task->shortDescription() << "\" ";
            ssTask << "start=\"" << task->startDate()->toQDateTime()->toString(Qt::ISODate).toStdString() << "\" ";
            ssTask << "end=\"" << task->endDate()->toQDateTime()->toString(Qt::ISODate).toStdString() << "\" ";
            ssTask << "timelength=\"" << task->totalTime()->toChar() << "\" ";

            stringstream periods;
            vector<TaskLog*>* logs = task->logs();
            DTime totalLogTime;
            periods << "<periods>\n";
            for (vector<TaskLog*>::iterator iterLog = logs->begin(); iterLog != logs->end(); iterLog++) {
                TaskLog* log = *iterLog;
                bool print = true;
                if (logsFrom) {
                    if (*log->start < *logsFrom) {
                        print = false;
                    }
                }
                if (logsTo) {
                    if (*log->end > *logsTo) {
                        print = false;
                    }
                }
                if (print) {
                    includeTask = true;
                    periods << "<period ";
                    periods << "id=\"" << *log->id << "\" ";
                    periods << "start=\"" << log->start->toQDateTime()->toString(Qt::ISODate).toStdString() << "\" ";
                    periods << "finish=\"" << log->end->toQDateTime()->toString(Qt::ISODate).toStdString() << "\" ";
                    periods << "timelength=\"" << log->totalTime()->toChar() << "\" />\n";
                    int logSecs = *log->end - *log->start;
                    totalLogTime.add(logSecs);
                    if (logSecs < 0) {
                        qDebug("Inconsistent log. Id: %s, Task Id: %s, Project: %s", log->id->c_str(), task->id()->c_str(), task->project()->name()->c_str());
                    }
                    totalProjectLogTime.add(logSecs);
                }
            }
            periods << "</periods>\n";
            ssTask << "logTime=\"" << totalLogTime.toChar() << "\">\n";
            ssTask << periods.str();
            ssTask << "</task>\n";
            if (includeTask) {
                ssTasks << ssTask.str();
            }
        }
        ssTasks << "</tasks>\n";
        result << " logTime=\"" << totalProjectLogTime.toChar() << "\" >\n";
        result << ssTasks.str();
        result << "</project>\n";
    }
    result << "</projects>\n";
    result.flush();
    result.close();
}
