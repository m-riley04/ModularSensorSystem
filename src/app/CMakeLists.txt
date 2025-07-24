cmake_minimum_required(VERSION 3.16)
project(
    app
    VERSION 0.0.1
    LANGUAGES CXX)

set(AUTOMOC ON)
set(AUTOUIC ON)
set(AUTORCC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(QT_QML_GENERATE_QMLLS_INI ON)
set(QT_QML_DEBUG ON)

# Sub-modules
add_subdirectory(styles)
add_subdirectory(logic)
add_subdirectory(components)
add_subdirectory(dialogs)
add_subdirectory(pages)

# Create the main executable
qt_add_executable(${PROJECT_NAME} main.cpp)

# Add QML module for the App itself
qt_add_qml_module(${PROJECT_NAME}
    URI ${PROJECT_NAME}
    VERSION 1.0
    RESOURCE_PREFIX "/qt/qml"
    OUTPUT_DIRECTORY "${QT_QML_OUTPUT_DIRECTORY}/${PROJECT_NAME}"
    QML_FILES
        main.qml
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        WIN32_EXECUTABLE TRUE
)

# Link with all required libraries including Core
target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Qt6::Quick
        Qt6::Qml
        Qt6::Core
        MSSDark
        actions
        components
        dialogs
        pages
        core # Link with the Core QML module
)

# Critical: Import QML plugins to make Core module available at runtime
qt_import_qml_plugins(${PROJECT_NAME})
