#include "closedtaskfilter.h"

#include "data.h"

ClosedTaskFilter::ClosedTaskFilter()
{
}

bool ClosedTaskFilter::acceptTask(Task* tsk) const {
    //return true;
    return (!tsk->isClosed());
}
