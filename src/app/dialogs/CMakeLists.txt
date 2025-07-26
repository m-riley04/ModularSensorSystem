qt_add_qml_module(dialogs
    URI app.dialogs
    VERSION 1.0
    OUTPUT_DIRECTORY "${QT_QML_OUTPUT_DIRECTORY}/app/dialogs"
    QML_FILES
        AddProcessorDialog.qml
        AddSourceDialog.qml
        SessionPropertiesDialog.qml
    IMPORTS
        QtQuick
        QtQuick.Controls
        QtQuick.Layouts
)
