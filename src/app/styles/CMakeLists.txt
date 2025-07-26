set_source_files_properties(Style.qml
    PROPERTIES QT_QML_SINGLETON_TYPE TRUE)

qt_add_qml_module(AppStylesLib
    URI app.styles
    VERSION 1.0
    DESIGNER_SUPPORTED
    QML_FILES
        Style.qml
    IMPORTS
        QtQuick
        QtQuick.Controls.Basic
)

add_subdirectory(MSSDark)
