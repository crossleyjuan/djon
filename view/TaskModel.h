/* 
 * File:   TaskModel.h
 * Author: cross
 *
 * Created on July 9, 2010, 9:06 AM
 */

#ifndef _TASKMODEL_H
#define	_TASKMODEL_H

#include "data.h"
#include "taskitem.h"
#include <QAbstractTableModel>
#include <vector>

class TaskModel : public QAbstractItemModel
 {
     Q_OBJECT

 public:
     TaskModel(const vector<Project*> projects, QObject *parent = 0);
     ~TaskModel();

     QVariant data(const QModelIndex &index, int role) const;
     Qt::ItemFlags flags(const QModelIndex &index) const;
     QVariant headerData(int section, Qt::Orientation orientation,
                         int role = Qt::DisplayRole) const;
     QModelIndex index(int row, int column,
                       const QModelIndex &parent = QModelIndex()) const;
     QModelIndex parent(const QModelIndex &index) const;
     int rowCount(const QModelIndex &parent = QModelIndex()) const;
     int columnCount(const QModelIndex &parent = QModelIndex()) const;
     bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);


     Task* task(const QModelIndex &index) const;
     Project* project(const QModelIndex &index) const;

 private:
     void setupModelData(TaskItem *parent);

     TaskItem *rootItem;
     std::vector<Project*> _projects;
 };

 #endif
