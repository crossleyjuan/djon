#ifndef DJONPLUGINS_GLOBAL_H
#define DJONPLUGINS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(DJONPLUGINS_LIBRARY)
#  define DJONPLUGINSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define DJONPLUGINSSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // DJONPLUGINS_GLOBAL_H