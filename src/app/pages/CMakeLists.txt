add_library(AppPages STATIC)

qt_add_qml_module(AppPages
    URI App.Pages
    VERSION 1.0
    QML_FILES
        MainPage.qml
    IMPORTS
        QtQuick
        QtQuick.Controls
        QtQuick.Window
    DEPENDENCIES TARGET 
        AppComponents
)