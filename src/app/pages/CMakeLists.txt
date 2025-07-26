qt_add_qml_module(AppPagesLib
    URI app.pages
    VERSION 1.0
    DESIGNER_SUPPORTED
    QML_FILES
        MainPage.qml
    IMPORTS
        QtQuick
        QtQuick.Controls
        QtQuick.Layouts
        app.components
    DEPENDENCIES 
        TARGET AppComponentsLib
)
