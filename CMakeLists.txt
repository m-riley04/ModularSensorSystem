cmake_minimum_required(VERSION 3.16)
project(ModularSensorSystem LANGUAGES CXX)

# Set C++20 standard globally
set(AUTOMOC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(QT_QML_GENERATE_QMLLS_INI ON)

# Find Qt6 components globally 
find_package(Qt6 REQUIRED COMPONENTS
    Core
    Quick
    Qml
    Multimedia
    Concurrent)

# Set up Qt6 for the entire project
qt_standard_project_setup(REQUIRES 6.8)

# Configure QML build output and import paths
set(QT_QML_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/qml")
set(QT_QML_IMPORT_PATH "${QT_QML_OUTPUT_DIRECTORY}")

# Build src
add_subdirectory(src)

# Ensure the application can locate QML modules at runtime
set_target_properties(app PROPERTIES
    QT_QML_IMPORT_PATH "${QT_QML_IMPORT_PATH}"
)

# Set App as the startup project (if using Visual Studio)
if(CMAKE_GENERATOR MATCHES "Visual Studio")
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT app)
endif()
