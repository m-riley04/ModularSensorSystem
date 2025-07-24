set(qml_singletons
    GlobalActions.qml
    PluginsActions.qml
    PresetsActions.qml
    ProcessorsActions.qml
    SessionActions.qml
    SourcesActions.qml
)
set_source_files_properties(${qml_singletons}
    PROPERTIES QT_QML_SINGLETON_TYPE TRUE)

qt_add_qml_module(actions
    URI app.logic.actions
    VERSION 1.0
    OUTPUT_DIRECTORY "${APP_QT_QML_OUTPUT_DIRECTORY}/app/logic/actions"
    RESOURCE_PREFIX "/qt/qml"
    QML_FILES
        ${qml_singletons}
    IMPORTS
        QtQuick
        QtQuick.Controls
)
