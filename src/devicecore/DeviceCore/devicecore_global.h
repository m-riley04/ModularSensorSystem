#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(DEVICECORE_LIB)
#  define DEVICECORE_EXPORT Q_DECL_EXPORT
# else
#  define DEVICECORE_EXPORT Q_DECL_IMPORT
# endif
#else
# define DEVICECORE_EXPORT
#endif
