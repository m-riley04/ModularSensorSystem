qt_add_qml_module(styles
    URI app.styles
    VERSION 1.0
    OUTPUT_DIRECTORY "${QT_QML_OUTPUT_DIRECTORY}/app/styles"
    RESOURCE_PREFIX "/qt/qml"
    QML_FILES
        Style.qml
    IMPORTS
        QtQuick
        QtQuick.Controls.Basic
)

add_subdirectory(MSSDark)
