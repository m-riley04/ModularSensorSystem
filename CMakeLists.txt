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

# Configure targets
set(TARGET_APP app)
set(TARGET_CORE core)

# Build src
add_subdirectory(src)