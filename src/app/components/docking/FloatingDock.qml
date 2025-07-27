import QtQuick
import QtQuick.Window


/**
  *
  */
Window {
  id: floater

  property var dockRoot
  // make the group accessible from outside without findChild()
  property alias group: floatingGroup
  property bool hasNativeWindowFrame: false

  width: 480
  height: 360
  title: floatingGroup.count > 0 ? ("Floating: " + floatingGroup.count + " tab(s)") : "Floating"
  visible: false
  flags: Qt.Window | Qt.WindowTitleHint | Qt.WindowSystemMenuHint | Qt.WindowMinMaxButtonsHint
         | (!floater.hasNativeWindowFrame ? Qt.FramelessWindowHint : null)

  DockGroup {
    id: floatingGroup
    anchors.fill: parent
    dockRoot: floater.dockRoot
    isFloating: true
  }

  // Auto-close when the last panel leaves
  Connections {
    target: floatingGroup
    function onCountChanged() {
      if (floatingGroup.count === 0) {
        // avoid re-entrancy issues; destroy the window after signal returns
        Qt.callLater(() => floater.destroy())
      }
    }
  }
}
