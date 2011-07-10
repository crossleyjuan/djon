#ifndef TASKDELEGATE_H
#define TASKDELEGATE_H

#include <QtGui>
#include "globaldefs.h"
#include "util.h"
class Project;
#include <vector>

class TaskDelegate : public QItemDelegate
{
public:
    TaskDelegate(const vector<Project*>* projects);
    void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;

private:
    void drawBackground(QPainter* painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    void drawTasks(QPainter* p, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    void drawTaskBar(QPainter *p, DateTime* startDate, DateTime* endDate, int dayWidth, const QStyleOptionViewItem & option) const;
    void drawGroupBar(QPainter *p, DateTime* barStartDate, DateTime* barEndDate, int dayWidth, const QStyleOptionViewItem & option) const;

    const vector<Project*>* _projects;
    QDate* _startDate;
    QDate* _endDate;
    SCALE _scale;
    int _totalDays;

// CONST
    static const int HEADER_HEIGHT = 30;
    static const int NUM_COLS = 20;
};

#endif // TASKDELEGATE_H
