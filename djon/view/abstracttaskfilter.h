#ifndef ABSTRACTTASKFILTER_H
#define ABSTRACTTASKFILTER_H

class Task;

enum TASKFILTER_TYPE {
    CLOSED_FILTER
};

class AbstractTaskFilter
{
public:
    AbstractTaskFilter(TASKFILTER_TYPE type) {
        _type = type;
    }

    AbstractTaskFilter(const AbstractTaskFilter& filter) {
        _type = filter._type;
    }

    virtual bool acceptTask(Task* tsk) const = 0;

    TASKFILTER_TYPE type() const {
        return _type;
    }

private:
    TASKFILTER_TYPE _type;
};

#endif // ABSTRACTTASKFILTER_H
