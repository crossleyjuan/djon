#ifndef CLOSEDTASKFILTER_H
#define CLOSEDTASKFILTER_H

#include "abstracttaskfilter.h"

class ClosedTaskFilter: public AbstractTaskFilter
{
public:
    ClosedTaskFilter();
    virtual bool acceptTask(Task* tsk) const;
};

#endif // CLOSEDTASKFILTER_H
