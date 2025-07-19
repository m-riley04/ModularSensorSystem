cmake_minimum_required(VERSION 3.16)
project(
    App
    VERSION 0.0.1
    LANGUAGES CXX)

include(qt.cmake)

set(QT_QML_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}")
set(QT_QML_GENERATE_QMLLS_INI ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(
    Qt6
    COMPONENTS
        Core
        Quick
        Qml
    REQUIRED
)

qt_standard_project_setup(REQUIRES 6.9)

# Sub-modules
add_subdirectory(components)
add_subdirectory(dialogs)
add_subdirectory(logic/actions)
add_subdirectory(pages)

qt_add_executable(${PROJECT_NAME} main.cpp)

qt_add_qml_module(${PROJECT_NAME}
    URI ${PROJECT_NAME}
    VERSION 1.0
    RESOURCE_PREFIX "/qt/qml"
    QML_FILES
        main.qml
    DEPENDENCIES TARGET
        AppComponents
        AppDialogs
        AppLogicActions
        AppPages
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        WIN32_EXECUTABLE TRUE
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Qt::Quick
        AppComponents
        AppDialogs
        AppLogicActions
        AppPages
)

if(Qt6_FOUND)
    qt_import_qml_plugins(${PROJECT_NAME})
endif()
