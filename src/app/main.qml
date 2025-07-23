import QtQuick
import QtQuick.Window
import QtQuick.Controls
import app.components
import app.logic.actions
import app.pages
import core

Window {
    id: window

    property int newFlags: Qt.Window | Qt.FramelessWindowHint

    visible: true
    width: 900
    height: 600
    title: "ModularSensorSystem"
    flags: window.newFlags

    Connections {
        target: GlobalActions.viewCustomWindowHandle
        function onTriggered() {
            GlobalActions.viewCustomWindowHandle.checked = !GlobalActions.viewCustomWindowHandle.checked;
            console.log(GlobalActions.viewCustomWindowHandle.checked);
            window.newFlags = GlobalActions.viewCustomWindowHandle.checked ? Qt.Window | Qt.FramelessWindowHint : Qt.Window;
        }
    }


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
