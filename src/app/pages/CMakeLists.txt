qt_add_qml_module(pages
    URI app.pages
    VERSION 1.0
    OUTPUT_DIRECTORY "${APP_QT_QML_OUTPUT_DIRECTORY}/pages"
    RESOURCE_PREFIX "/qt/qml"
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
