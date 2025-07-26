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

# Configure QML build output and import paths
set(QT_QML_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/qml")
set(QT_QML_IMPORT_PATH "${QT_QML_OUTPUT_DIRECTORY}")

# Configure targets
set(TARGET_APP app)
set(TARGET_CORE core)

# Build src
add_subdirectory(src)

# Ensure the application can locate QML modules at runtime
set_target_properties(${TARGET_APP} PROPERTIES
    QT_QML_IMPORT_PATH "${QT_QML_IMPORT_PATH}"
)
