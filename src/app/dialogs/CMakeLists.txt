add_library(AppDialogs STATIC)

qt_add_qml_module(AppDialogs
    URI App.Dialogs
    VERSION 1.0
    QML_FILES
        AddProcessorDialog.qml
        AddSourceDialog.qml
        SessionPropertiesDialog.qml
    IMPORTS
        QtQuick
        QtQuick.Controls
        QtQuick.Layouts
)