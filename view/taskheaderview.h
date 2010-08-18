#ifndef TASKHEADERVIEW_H
#define TASKHEADERVIEW_H

#include <QtGui>
#include <vector>
class Project;
class DateTime;

class TaskHeaderView : public QHeaderView
{
Q_OBJECT
public:
    explicit TaskHeaderView(std::vector<Project*>* projects, Qt::Orientation orientation, QWidget *parent = 0);
    int sectionSizeHint(int logicalIndex) const;
    int sectionSize(int logicalIndex) const;
    void refresh();
    void setProjects(std::vector<Project*>* projects);
//    int length() const;
//    QSize sizeHint() const;
//    int minimumSectionSize() const;


protected:
    void paintSection ( QPainter * painter, const QRect & rect, int logicalIndex ) const;

signals:

public slots:

private:
    std::vector<Project*>* _projects;
    DateTime* _startDate;
    DateTime* _endDate;
//    SCALE _scale;
    int _totalDays;


};

#endif // TASKHEADERVIEW_H
