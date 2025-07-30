qt_add_qml_module(AppComponentsPluginsLib
    URI app.components.plugins
    VERSION 1.0
    DESIGNER_SUPPORTED
    IMPORTS
        QtQuick
        QtQuick.Controls
        QtQuick.Layouts
        QtQuick.Window
    DEPENDENCIES
        TARGET AppStylesLib
    QML_FILES
    DialogWindow.qml
)
