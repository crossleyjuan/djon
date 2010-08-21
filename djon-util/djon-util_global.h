#ifndef DJONUTIL_GLOBAL_H
#define DJONUTIL_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(DJONUTIL_LIBRARY)
#  define DJONUTILSHARED_EXPORT Q_DECL_EXPORT
#else
#  define DJONUTILSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // DJONUTIL_GLOBAL_H
