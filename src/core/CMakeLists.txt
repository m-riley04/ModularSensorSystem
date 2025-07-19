cmake_minimum_required(VERSION 3.16)
project(core LANGUAGES CXX)

include(qt.cmake)

qt_standard_project_setup()

file(
    GLOB
    HEADER_SOURCES
    "*.h")
file(
    GLOB
    CPP_SOURCES
    "*.cpp")

find_package(
    Qt6 
    COMPONENTS
        Core
        Multimedia
        Qml
        Concurrent
    REQUIRED)

qt_add_library(
    ${PROJECT_NAME} SHARED
    ${HEADER_SOURCES}
    ${CPP_SOURCES}
)

# Make headers available to consumers
target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link against QtCore (and anything else backend needs)
target_link_libraries(${PROJECT_NAME}
    PUBLIC Qt::Core Qt::Multimedia Qt::Qml Qt::Concurrent
)

# Nice-to-have on Windows: export every symbol automatically
set_target_properties(${PROJECT_NAME} PROPERTIES
    WINDOWS_EXPORT_ALL_SYMBOLS ON
)