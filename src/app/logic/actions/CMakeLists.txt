qt_add_qml_module(actions
    URI app.logic.actions
    VERSION 1.0
    QML_FILES
        GlobalActions.qml
        PluginsActions.qml
        PresetsActions.qml
        ProcessorsActions.qml
        SessionActions.qml
        SourcesActions.qml
    IMPORTS
        QtQuick
        QtQuick.Controls
        QtQuick.Window
)
