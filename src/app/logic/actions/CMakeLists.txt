qt_add_qml_module(actions
    URI app.logic.actions
    VERSION 1.0
    OUTPUT_DIRECTORY "${APP_QT_QML_OUTPUT_DIRECTORY}/logic/actions"
    RESOURCE_PREFIX "/qt/qml"
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
)
