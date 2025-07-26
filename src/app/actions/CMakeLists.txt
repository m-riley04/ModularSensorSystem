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
    URI app.actions
    VERSION 1.0
    OUTPUT_DIRECTORY "${QT_QML_OUTPUT_DIRECTORY}/app/actions"
    QML_FILES
        ${qml_singletons}
    IMPORTS
        QtQuick
        QtQuick.Controls
)
