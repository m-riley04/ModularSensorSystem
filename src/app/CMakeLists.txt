project(
    app
    VERSION 1.0.0
    LANGUAGES CXX)

# Sub-modules
add_subdirectory(styles)
add_subdirectory(logic)
add_subdirectory(components)
add_subdirectory(dialogs)
add_subdirectory(pages)

# Create the main executable
qt_add_executable(${PROJECT_NAME} main.cpp)

# Add QML module for the app
qt_add_qml_module(${PROJECT_NAME}
    URI ${PROJECT_NAME}
    VERSION 1.0
    RESOURCE_PREFIX "/qt/qml"
    OUTPUT_DIRECTORY "${QT_QML_OUTPUT_DIRECTORY}/${PROJECT_NAME}"
    QML_FILES
        main.qml
    IMPORTS
        QtQuick
        QtQuick.Controls
        QtQuick.Window
        app.components
        app.pages
        app.logic.actions
        app.styles.MSSDark
        core
)

# Set target props for main app
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
        core
)


