#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(MSSFRONTENDWIDGETS_LIB)
#  define MSSFRONTENDWIDGETS_EXPORT Q_DECL_EXPORT
# else
#  define MSSFRONTENDWIDGETS_EXPORT Q_DECL_IMPORT
# endif
#else
# define MSSFRONTENDWIDGETS_EXPORT
#endif
