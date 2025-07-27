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

  // Public API
  function dockTo(group) {
    if (!canUndock)
      return
    if (!group || group === currentGroup)
      return
    if (currentGroup)
      currentGroup.removePanel(root)
    group.addPanel(root)
    currentGroup = group
  }
  signal undockRequested

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
          enabled: root.canDrag
          acceptedButtons: Qt.LeftButton
          target: null // we’re not moving this Item directly
          grabPermissions: PointerHandler.TakeOverForbidden
          onActiveChanged: {
            if (!root.dockRoot)
              return
            if (active) {
              root.dockRoot.beginDrag(root)
            } else {
              const pScene = header.mapToItem(null,
                                              centroid.position) // scene coords
              root.dockRoot.endDrag(root, pScene)
            }
          }
          onCentroidChanged: {
            if (!active || !root.dockRoot)
              return
            const pScene = header.mapToItem(null,
                                            centroid.position) // scene coords
            root.dockRoot.updateDragPosition(pScene)
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
