qt_add_qml_module(styles
    URI app.styles
    VERSION 1.0
    QML_FILES
        Style.qml
    IMPORTS
        QtQuick
        QtQuick.Controls.Basic
)

add_subdirectory(MSSDark)
