#include "closedtaskfilter.h"

#include "data.h"

ClosedTaskFilter::ClosedTaskFilter()
    : AbstractTaskFilter(CLOSED_FILTER)
{
}

bool ClosedTaskFilter::acceptTask(Task* tsk) const {
    //return true;
    return (!tsk->isClosed());
}
