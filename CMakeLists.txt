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

# Build order: Core library first, then App that depends on it
add_subdirectory(src/core)
add_subdirectory(src/app)

# Set App as the startup project (if using Visual Studio)
if(CMAKE_GENERATOR MATCHES "Visual Studio")
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT app)
endif()
