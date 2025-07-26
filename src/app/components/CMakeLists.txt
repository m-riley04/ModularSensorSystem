qt_add_qml_module(AppComponentsLib
    URI app.components
    VERSION 1.0
    QML_FILES
        CustomTitleBar.qml
        PresetsWidget.qml
        ProcessorsWidget.qml
        SessionControlsWidget.qml
        SourcePreviewWidget.qml
        SourcesWidget.qml
        HomeToolBar.qml
        HomeMenuBar.qml
    IMPORTS
        QtQuick
        QtQuick.Controls
        QtQuick.Layouts
        app.actions
        app.styles.MSSDark
    DEPENDENCIES
        TARGET AppActionsLib
        TARGET AppStylesMSSDarkLib
)
