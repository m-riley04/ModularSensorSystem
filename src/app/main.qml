import QtQuick
import QtQuick.Window
import QtQuick.Controls
import app.components
import app.pages
import core

Window {
    id: window

    visible: true
    width: 900
    height: 600
    title: "ModularSensorSystem"
    flags: Qt.Window | Qt.FramelessWindowHint

    CustomTitleBar {
        id: titleBar
        width: window.width
        title: `Number of subcontrollers: ${MainController.numSubControllers}`
    }

    HomeMenuBar {
        id: homeMenuBar
        anchors.top: titleBar.bottom
        width: window.width
    }

    HomeToolBar {
        id: homeToolBar
        anchors.top: homeMenuBar.bottom
        width: window.width
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
