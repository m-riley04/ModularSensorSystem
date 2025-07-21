qt_add_qml_module(dialogs
    URI app.dialogs
    VERSION 1.0
    OUTPUT_DIRECTORY "${APP_QT_QML_OUTPUT_DIRECTORY}/dialogs"
    RESOURCE_PREFIX "/qt/qml"
    QML_FILES
        AddProcessorDialog.qml
        AddSourceDialog.qml
        SessionPropertiesDialog.qml
    IMPORTS
        QtQuick
        QtQuick.Controls
        QtQuick.Layouts
)
