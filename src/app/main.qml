import QtQuick
import QtQuick.Window
import QtQuick.Controls
import app.components
import app.pages
import app.logic.actions
import core

Window {
    id: window

    // Access MainController singleton directly
    property MainController mainController: MainController

    visible: true
    width: 900
    height: 600
    title: "ModularSensorSystem"
    flags: Qt.Window | Qt.FramelessWindowHint

    // Custom Title Bar (placeholder, can be replaced with a custom QML component)
    CustomTitleBar {
        id: titleBar
        title: `Number of subcontrollers: ${window.mainController.numSubControllers}`
    }

    // Menu Bar
    HomeMenuBar {
        id: homeMenuBar
        anchors.top: titleBar.bottom
    }

    HomeToolBar {
        id: homeToolBar
        anchors.top: homeMenuBar.bottom
        width: parent.width
    }

    // Main Page Stack
    StackView {
        id: pagesStack
        anchors.top: homeToolBar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        MainPage {}
    }
}
