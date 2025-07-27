import QtQuick
import QtQuick.Window
import QtQuick.Controls
import app.components
import app.actions
import app.pages
import core
import app.styles.MSSDark
import app.components.docking

Window {
  id: window

  property int newFlags: Qt.Window | Qt.FramelessWindowHint
  property bool customWindow: true

  visible: true
  width: 900
  height: 600
  title: "ModularSensorSystem"
  flags: window.newFlags

  // Connections for menubar
  Connections {
    target: GlobalActions.viewCustomWindowHandle
    function onTriggered(state) {
      // For some reason, "onToggled" doesn't work. So instead, I'm manually storing the custom window state.
      window.customWindow = !window.customWindow
      GlobalActions.viewCustomWindowHandle.checked = window.customWindow
      console.log(window.customWindow)
      window.newFlags = window.customWindow ? Qt.Window | Qt.FramelessWindowHint : Qt.Window
    }
  }

  CustomTitleBar {
    id: titleBar
    width: window.width
    title: `${window.title} (${MainController.numSubControllers})`
    visible: window.customWindow
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
