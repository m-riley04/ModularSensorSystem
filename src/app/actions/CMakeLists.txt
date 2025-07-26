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

qt_add_qml_module(AppActionsLib
    URI app.actions
    VERSION 1.0
    DESIGNER_SUPPORTED
    QML_FILES
        ${qml_singletons}
    IMPORTS
        QtQuick
        QtQuick.Controls
)
