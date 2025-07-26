# Sub-modules
add_subdirectory(styles)
add_subdirectory(logic)
add_subdirectory(components)
add_subdirectory(dialogs)
add_subdirectory(pages)

# Create the main executable
qt_add_executable(${TARGET_APP} main.cpp)

# Add QML module for the app
qt_add_qml_module(${TARGET_APP}
    URI ${TARGET_APP}
    VERSION 1.0
    RESOURCE_PREFIX "/qt/qml"
    OUTPUT_DIRECTORY "${QT_QML_OUTPUT_DIRECTORY}/${TARGET_APP}"
    QML_FILES
        main.qml
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
        MSSDark
        actions
        components
        dialogs
        pages
        core
)


