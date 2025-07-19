add_library(AppComponents STATIC)

qt_add_qml_module(AppComponents
    URI App.Components
    VERSION 1.0
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