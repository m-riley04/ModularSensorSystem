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
    IMPORTS
        QtQuick
        QtQuick.Controls
        QtQuick.Layouts
)
