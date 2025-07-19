add_library(AppLogicActions STATIC)

qt_add_qml_module(AppLogicActions
    URI App.Logic.Actions
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