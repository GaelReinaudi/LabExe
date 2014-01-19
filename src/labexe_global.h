#ifndef LABEXE_GLOBAL_H
#define LABEXE_GLOBAL_H

#include <qglobal.h>
#include <QApplication>

#ifdef LABEXE_LIB
# define LABEXE_EXPORT Q_DECL_EXPORT
#else
# define LABEXE_EXPORT Q_DECL_IMPORT
#endif

#endif // LABEXE_GLOBAL_H
