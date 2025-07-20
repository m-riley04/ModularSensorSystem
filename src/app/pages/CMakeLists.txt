qt_add_qml_module(pages
    URI app.pages
    VERSION 1.0
    QML_FILES
        MainPage.qml
    IMPORTS
        QtQuick
        QtQuick.Controls
        QtQuick.Layouts
        app.components
    DEPENDENCIES TARGET
        components
)
