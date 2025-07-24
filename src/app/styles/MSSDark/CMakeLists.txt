qt_add_qml_module(MSSDark
    URI app.styles.MSSDark
    VERSION 1.0
    OUTPUT_DIRECTORY "${QT_QML_OUTPUT_DIRECTORY}/app/styles/MSSDark"
    RESOURCE_PREFIX "/qt/qml"
    QML_FILES
        Button.qml
    IMPORTS
        QtQuick
        QtQuick.Controls.Basic
)
