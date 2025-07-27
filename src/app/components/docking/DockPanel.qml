import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
  id: root
  implicitWidth: 360
  implicitHeight: 240

  // Set by DockRoot when added
  property var dockRoot
  property var currentGroup

  // Properties set to panel
  property string title: "Panel"
  property bool canUndock: true
  property bool canClose: true
  property bool canDrag: true
  property alias contentItem: contentHost

  readonly property bool isFloating: !!(currentGroup && currentGroup.isFloating)
  signal undockRequested


  /**
    * Docks this panel to a group.
    */
  function dockTo(group) {
    if (!canUndock || !group || group === currentGroup)
      return

    if (currentGroup)
      currentGroup.removePanel(root)
    group.addPanel(root)
    currentGroup = group
  }

  Rectangle {
    id: frame
    anchors.fill: parent
    color: Qt.rgba(0.10, 0.10, 0.12, 1)
    border.color: Qt.rgba(1, 1, 1, 0.08)
    radius: 6

    ColumnLayout {
      anchors.fill: parent
      spacing: 0

      // Header (drag handle + controls)
      Rectangle {
        id: header
        Layout.fillWidth: true
        Layout.preferredHeight: 36
        radius: 6
        color: Qt.rgba(1, 1, 1, 0.05)
        border.color: Qt.rgba(1, 1, 1, 0.06)

        RowLayout {
          anchors.fill: parent
          anchors.margins: 8
          spacing: 8

          Label {
            id: titleLabel
            color: "#ffffff"
            text: root.title
            font.bold: true
            elide: Label.ElideRight
            Layout.fillWidth: true
          }

          ToolButton {
            text: "\u2197" // ↗ undock
            ToolTip.text: "Undock (float)"
            onClicked: root.canUndock ? root.undockRequested() : null
            visible: root.canUndock
          }

          ToolButton {
            text: "\u2715"
            ToolTip.text: "Close"
            onClicked: {
              if (!root.canClose)
                return
              if (root.currentGroup)
                root.currentGroup.removePanel(root)
              root.destroy(0)
            }
            visible: root.canClose
          }
        }

        // Drag from header
        DragHandler {
          id: drag
          target: null
          acceptedButtons: Qt.LeftButton

          // use a permissive grab policy so drags starting on header text still work
          // (omit this line to use Qt's default, which is already permissive)
          // grabPermissions: PointerHandler.CanTakeOverFromItems
          property point lastGlobal

          onActiveChanged: {
            if (!root.dockRoot)
              return
            const g = header.mapToGlobal(centroid.position.x,
                                         centroid.position.y)
            lastGlobal = g
            if (active) {
              // Always begin a docking drag (we may end up re-docking)
              root.dockRoot.beginDrag(root)
            } else {
              // Always finish the docking drag (docked or floating)
              root.dockRoot.endDrag(root, g)
            }
          }

          onCentroidChanged: {
            if (!active || !root.dockRoot)
              return

            const g = header.mapToGlobal(centroid.position.x,
                                         centroid.position.y)

            if (root.isFloating) {
              // While floating: manually move the whole floating window
              const win = header.Window.window
              if (win) {
                win.x += (g.x - lastGlobal.x)
                win.y += (g.y - lastGlobal.y)
              }
            }

            // In both cases, feed global point to docking hit-tests
            root.dockRoot.updateDragPosition(g)
            lastGlobal = g
          }
        }
      }

      // Content area (default data slot)
      Item {
        id: contentHost
        Layout.fillWidth: true
        Layout.fillHeight: true
        clip: true
        default property alias content: contentHost.data
      }
    }
  }
}
