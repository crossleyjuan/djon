#include "exportutility.h"
#include "data.h"
#include <fstream>

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
        result << "totalTime=\"" << proj->totalTime()->toChar() << "\" >\n";

        result << "<tasks>\n";
        vector<Task*>* tasks = proj->tasks();
        for (vector<Task*>::iterator iterTask = tasks->begin(); iterTask != tasks->end(); iterTask++) {
            Task* task = *iterTask;
            result << "<task ";
            result << "number=\"" << *task->id() << "\" ";
            result << "name=\"" << *task->shortDescription() << "\" ";
            result << "start=\"" << task->startDate()->toQDateTime()->toString(Qt::ISODate).toStdString() << "\" ";
            result << "end=\"" << task->endDate()->toQDateTime()->toString(Qt::ISODate).toStdString() << "\" ";
            result << "timelength=\"" << task->totalTime()->toChar() << "\">\n";

            vector<TaskLog*>* logs = task->logs();
            result << "<periods>\n";
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
                    result << "<period ";
                    result << "id=\"" << *log->id << "\" ";
                    result << "start=\"" << log->start->toQDateTime()->toString(Qt::ISODate).toStdString() << "\" ";
                    result << "finish=\"" << log->end->toQDateTime()->toString(Qt::ISODate).toStdString() << "\" ";
                    result << "timelength=\"" << log->totalTime()->toChar() << "\" />\n";
                }
            }
            result << "</periods>\n";
            result << "</task>\n";
        }
        result << "</tasks>\n";
        result << "</project>\n";
    }
    result << "</projects>\n";
    result.flush();
    result.close();
}
