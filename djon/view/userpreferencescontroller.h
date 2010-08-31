#ifndef USERPREFERENCESCONTROLLER_H
#define USERPREFERENCESCONTROLLER_H

#include <QObject>
#include <QModelIndex>
#include <vector>

class Task;
class TaskModel;

class UserPreferencesController : public QObject
{
    Q_OBJECT
public:
    UserPreferencesController(const TaskModel* model);

public slots:
    void collapsed(const QModelIndex index);
    void expanded(const QModelIndex& index);

private:
    const TaskModel* _model;

};

#endif // USERPREFERENCESCONTROLLER_H
