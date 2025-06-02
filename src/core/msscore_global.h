#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(MSSCORE_LIB)
#  define MSSCORE_EXPORT Q_DECL_EXPORT
# else
#  define MSSCORE_EXPORT Q_DECL_IMPORT
# endif
#else
# define MSSCORE_EXPORT
#endif
