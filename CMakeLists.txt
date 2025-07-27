cmake_minimum_required(VERSION 3.22)
project(
    ModularSensorSystem
    VERSION 1.0.0
    LANGUAGES CXX)

# Find Qt6 components globally 
find_package(Qt6 6.8 REQUIRED COMPONENTS
    Core
    Quick
    Qml
    Multimedia
    Concurrent)

# Set up Qt6 for the entire project
qt_standard_project_setup(REQUIRES 6.8)

# Set C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Generate QMLLS_INI files
set(QT_QML_GENERATE_QMLLS_INI ON)

# Configure main library/bin targets
set(TARGET_APP AppBin)
set(TARGET_CORE CoreLib)

# Where CMake writes auto-generated qmldir/qmltypes (per-URI under this root)
set(QT_QML_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/qml")

# Tell Qt Creator/Designer where to find QML modules while editing
#  - ${CMAKE_SOURCE_DIR}/src : source root that contains 'app/', 'core/', etc. (module roots)
#  - ${CMAKE_BINARY_DIR}/qml : matches QT_QML_OUTPUT_DIRECTORY above
set(QML_IMPORT_PATH
    "${CMAKE_SOURCE_DIR}"         # contains 'app', 'core', etc. (module roots)
    "${CMAKE_BINARY_DIR}/qml"     # matches QT_QML_OUTPUT_DIRECTORY above
    CACHE STRING "Qt Creator extra QML import paths" FORCE
)

# Build src
add_subdirectory(src)
add_subdirectory(tests/app_tests)
