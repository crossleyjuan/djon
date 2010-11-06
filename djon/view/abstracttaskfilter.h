#ifndef ABSTRACTTASKFILTER_H
#define ABSTRACTTASKFILTER_H

class Task;

class AbstractTaskFilter
{
public:
    AbstractTaskFilter() {

    }

    AbstractTaskFilter(const AbstractTaskFilter& filter) {

    }

    virtual bool acceptTask(Task* tsk) const = 0;
};

#endif // ABSTRACTTASKFILTER_H
