# Sub-modules
add_subdirectory(styles)
add_subdirectory(actions)
add_subdirectory(components)
add_subdirectory(dialogs)
add_subdirectory(pages)

# Create the main executable
qt_add_executable(${TARGET_APP} main.cpp)

# Add QML module for the app
qt_add_qml_module(${TARGET_APP}
    URI app
    VERSION 1.0
    QML_FILES
        Main.qml
    IMPORTS
        QtQuick
        QtQuick.Controls
        QtQuick.Layouts
        QtQuick.Window
        app.actions
        app.components
        app.dialogs
        app.pages
        app.components.docking
    DEPENDENCIES
        TARGET ${TARGET_CORE}
        TARGET AppStylesMSSDarkLib
        TARGET AppStylesLib
        TARGET AppActionsLib
        TARGET AppComponentsLib
        TARGET AppDialogsLib
        TARGET AppPagesLib
        TARGET AppComponentsDockingLib
    RESOURCES
        media/icons/app.png
)

# Set target props for main app
set_target_properties(${TARGET_APP}
    PROPERTIES
        WIN32_EXECUTABLE TRUE
)

# Link with all required libraries including Core
target_link_libraries(${TARGET_APP}
    PUBLIC
        Qt6::Quick
        Qt6::Qml
        Qt6::Core
        Qt6::Multimedia
        ${TARGET_CORE}
        AppStylesMSSDarkLib
        AppStylesLib
        AppActionsLib
        AppComponentsLib
        AppDialogsLib
        AppPagesLib
)


