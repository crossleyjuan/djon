#include "logworkreportscence.h"

LogWorkReportScence::LogWorkReportScence(QObject *parent) :
    QGraphicsScene(parent)
{
}

LogWorkReportScence::~LogWorkReportScence() {

}

void LogWorkReportScence::setModel(TaskModel* model) {
    _model = model;
}

void LogWorkReportScence::setupScene() {
    clear();


}
