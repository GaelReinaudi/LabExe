#ifndef PLUGINPHIDGETS_GLOBAL_H
#define PLUGINPHIDGETS_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef PLUGINPHIDGETS_LIB
# define PLUGINPHIDGETS_EXPORT Q_DECL_EXPORT
#else
# define PLUGINPHIDGETS_EXPORT Q_DECL_IMPORT
#endif

#endif // PLUGINPHIDGETS_GLOBAL_H