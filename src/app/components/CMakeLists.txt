qt_add_qml_module(components
    URI app.components
    VERSION 1.0
    OUTPUT_DIRECTORY "${QT_QML_OUTPUT_DIRECTORY}/app/components"
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
        TARGET actions
        TARGET MSSDark
)
