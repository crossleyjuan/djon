#ifndef TASKCOMBOBOX_H
#define TASKCOMBOBOX_H

#include <QComboBox>
#include <QObject>
#include <vector>

class Project;
class Task;

class ComboItem {
private:
    int _id;
    bool _taskElement; // false if it's a project
    void* _element;
public:
    ComboItem(int id, bool taskElement, void* element) {
        _id = id;
        _taskElement = taskElement;
        _element = element;
    }

    void* element() const {
        return _element;
    }

    bool taskElement() const {
        return _taskElement;
    }

    int id() const {
        return _id;
    }
};

class TaskComboBox : public QComboBox
{
    Q_OBJECT
public:
    TaskComboBox(std::vector<Project*>* projects, QWidget* parent = 0);
    void refresh(std::vector<Project*>* projects);

private:
    void initialize();
    void insertTask(Task* task, int id, int &indent);

    std::vector<Project*>* _projects;
    std::map<int, ComboItem*> _pointers;

};

#endif // TASKCOMBOBOX_H
