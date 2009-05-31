#ifndef GANTT_GLOBAL_H
#define GANTT_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(GANTT_LIBRARY)
#  define GANTTSHARED_EXPORT Q_DECL_EXPORT
#else
#  define GANTTSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // GANTT_GLOBAL_H
