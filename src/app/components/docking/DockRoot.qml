import QtQuick
import QtQuick.Layouts
import QtQuick.Window

Item {
  id: root

  property var extraDropTargets: [] // list any DockRow you create

  function floatPanel(panel, x, y) {
    const f = floaterComponent.createObject(root, {
                                              "dockRoot": root,
                                              "x": x !== undefined ? x : panel.x + 60,
                                              "y": y !== undefined ? y : panel.y + 60
                                            })
    if (panel.currentGroup)
      panel.currentGroup.removePanel(panel)
    f.group.addPanel(panel) // use the alias you exposed
    f.visible = true // show after attaching content
  }

  // Public API: add a panel to any zone (default center)
  function addPanel(panel, zone) {
    panel.dockRoot = root
    switch (zone) {
    case Docking.DockZone.LEFT:
      leftGroup.addPanel(panel)
      break
    case Docking.DockZone.RIGHT:
      rightGroup.addPanel(panel)
      break
    case Docking.DockZone.TOP:
      topGroup.addPanel(panel)
      break
    case Docking.DockZone.BOTTOM:
      bottomGroup.addPanel(panel)
      break
    default:
      centerGroup.addPanel(panel)
    }
  }

  // === Drag/drop lifecycle ===
  property var draggingPanel: null
  function beginDrag(panel) {
    draggingPanel = panel
    for (const tgt of extraDropTargets)
      tgt.beginDrag(panel)
    overlay.visible = true
  }
  function updateDragPosition(pScene) {
    let anyRowHover = false
    for (const tgt of extraDropTargets) {
      tgt.updateDrag(pScene)
      if (tgt.isUnderDrag)
        anyRowHover = true
    }
    if (anyRowHover) {
      overlay.visible = false
      overlay.targetZone = Docking.DockZone.NONE
    } else {
      overlay.visible = true
      overlay.targetZone = overlay.hitTest(pScene)
    }
  }
  function endDrag(panel, pScene) {
    if (panel !== draggingPanel)
      return
    // If any row consumed it, stop here
    for (const tgt of extraDropTargets) {
      if (tgt.isUnderDrag) {
        tgt.endDrag(panel, pScene)
        overlay.visible = false
        draggingPanel = null
        return
      }
    }
    // Row did not consume; proceed with zone overlay
    for (const tgt of extraDropTargets)
      tgt.endDrag(panel, pScene)
    const zone = overlay.hitTest(pScene)
    overlay.visible = false
    draggingPanel = null

    switch (zone) {
    case Docking.DockZone.LEFT:
      panel.dockTo(leftGroup)
      break
    case Docking.DockZone.RIGHT:
      panel.dockTo(rightGroup)
      break
    case Docking.DockZone.TOP:
      panel.dockTo(topGroup)
      break
    case Docking.DockZone.BOTTOM:
      panel.dockTo(bottomGroup)
      break
    case Docking.DockZone.CENTER:
      panel.dockTo(centerGroup)
      break
    default:
      floatPanel(panel)
    }
  }
  function registerDropTarget(t) {
    if (t && extraDropTargets.indexOf(t) < 0)
      extraDropTargets.push(t)
  }
  function unregisterDropTarget(t) {
    const i = extraDropTargets.indexOf(t)
    if (i >= 0)
      extraDropTargets.splice(i, 1)
  }

  Component {
    id: floaterComponent
    FloatingDock {}
  }

  // Layout: top / center (left|center|right) / bottom
  ColumnLayout {
    anchors.fill: parent
    spacing: 6

    // Top band
    Item {
      Layout.fillWidth: true
      Layout.preferredHeight: topGroup.count > 0 ? 220 : 0
      Behavior on Layout.preferredHeight {
        NumberAnimation {
          duration: 150
        }
      }
      DockGroup {
        id: topGroup
        anchors.fill: parent
        dockRoot: root
      }
    }

    // Middle band: left / center / right
    RowLayout {
      Layout.fillWidth: true
      Layout.fillHeight: true
      spacing: 6

      Item {
        Layout.preferredWidth: leftGroup.count > 0 ? 320 : 0
        Layout.fillHeight: true
        Behavior on Layout.preferredWidth {
          NumberAnimation {
            duration: 150
          }
        }
        DockGroup {
          id: leftGroup
          anchors.fill: parent
          dockRoot: root
        }
      }

      Rectangle {
        Layout.fillWidth: true
        Layout.fillHeight: true
        radius: 8
        color: Qt.rgba(1, 1, 1, 0.02)
        border.color: Qt.rgba(1, 1, 1, 0.05)
        DockGroup {
          id: centerGroup
          anchors.fill: parent
          dockRoot: root
        }
      }

      Item {
        Layout.preferredWidth: rightGroup.count > 0 ? 320 : 0
        Layout.fillHeight: true
        Behavior on Layout.preferredWidth {
          NumberAnimation {
            duration: 150
          }
        }
        DockGroup {
          id: rightGroup
          anchors.fill: parent
          dockRoot: root
        }
      }
    }

    // Bottom band
    Item {
      Layout.fillWidth: true
      Layout.preferredHeight: bottomGroup.count > 0 ? 220 : 0
      Behavior on Layout.preferredHeight {
        NumberAnimation {
          duration: 150
        }
      }
      DockGroup {
        id: bottomGroup
        anchors.fill: parent
        dockRoot: root
      }
    }
  }

  // Overlay shown during drags
  DockOverlay {
    id: overlay
  }
}
