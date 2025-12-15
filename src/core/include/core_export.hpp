#pragma once

#include <QtCore/qglobal.h>

// MSS Core library export/import macro
// Define MSSCORE_LIB when building the MSSCore DLL
// Consumers of the DLL should NOT define MSSCORE_LIB

#ifdef _WIN32
    #ifdef MSSCORE_LIB
        #define MSS_CORE_API Q_DECL_EXPORT
    #else
        #define MSS_CORE_API Q_DECL_IMPORT
    #endif
#else
    // For non-Windows platforms (GCC/Clang)
    #ifdef MSSCORE_LIB
        #define MSS_CORE_API __attribute__((visibility("default")))
    #else
        #define MSS_CORE_API
    #endif
#endif
