#ifndef ABSTRACTTASKMODEL_H
#define ABSTRACTTASKMODEL_H

#include <QModelIndex>
#include <QAbstractItemModel>

class Task;
class Project;

class AbstractTaskModel: public QAbstractItemModel
{
public:
    AbstractTaskModel();


    virtual Task* task(const QModelIndex &index) const = 0;
    virtual Project* project(const QModelIndex &index) const = 0;
};

#endif // ABSTRACTTASKMODEL_H
