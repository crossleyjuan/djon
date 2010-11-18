#include "logworkreportview.h"

LogWorkReportView::LogWorkReportView(QWidget *parent)
    : QGraphicsView(parent) {

}

LogWorkReportView::LogWorkReportView(QGraphicsScene *scene, QWidget *parent)
    : QGraphicsView(scene, parent) {

}

LogWorkReportView::~LogWorkReportView() {

}

void LogWorkReportView::setTaskModel(TaskModel* model) {
    _taskModel = model;
}
