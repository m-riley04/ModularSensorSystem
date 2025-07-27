import QtQuick
import QtQuick.Layouts
import QtQuick.Window

Item {
  id: root

  property var extraDropTargets: [] // list any DockRow you create


  /**
    * Makes a floating window for a panel at a specific global position.
    */
  function floatPanel(panel, posGlobal) {
    // Only undock/float panels that are allowed to.
    if (!panel || !panel.canUndock)
      return

    // map global to this root's coords so the new window appears under the cursor
    const p = root.mapFromGlobal(posGlobal.x, posGlobal.y)
    const offsetX = -60
    const offsetY = -20
    // Item.mapFromGlobal() is on Item. :contentReference[oaicite:0]{index=0}
    const f = floaterComponent.createObject(root, {
                                              "dockRoot": root
                                            })
    if (panel.currentGroup)
      panel.currentGroup.removePanel(panel)
    f.group.addPanel(panel)
    f.visible = true
  }


  /**
    * Adds a panel to a specific zone
    * @param panel The panel to add
    * @param zone The zone to add the panel to. Default is center.
    */
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

  // Reference to the panel being dragged
  property var draggingPanel: null


  /**
    * Called at the beginning of the drag event.
    */
  function beginDrag(panel) {
    draggingPanel = panel
    for (const tgt of extraDropTargets)
      tgt.beginDrag(panel)
    overlay.visible = true
  }


  /**
    * Called during the drag event to update the position.
    */
  function updateDragPosition(posGlobal) {
    let anyRowHover = false
    for (const tgt of extraDropTargets) {
      tgt.updateDrag(posGlobal)
      if (tgt.isUnderDrag)
        anyRowHover = true
    }
    if (anyRowHover) {
      overlay.visible = false
      overlay.targetZone = Docking.DockZone.NONE
    } else {
      overlay.visible = true
      const pOverlay = overlay.mapFromGlobal(posGlobal.x, posGlobal.y)
      overlay.targetZone = overlay.hitTest(posGlobal)
    }
  }


  /**
    * Called at the end of the drag event (when it's dropped).
    */
  function endDrag(panel, posGlobal) {
    if (panel !== draggingPanel)
      return
    // If any row consumed it, stop here
    for (const tgt of extraDropTargets) {
      if (tgt.isUnderDrag) {
        tgt.endDrag(panel, posGlobal)
        overlay.visible = false
        draggingPanel = null
        return
      }
    }

    const pOverlay = overlay.mapFromGlobal(posGlobal.x, posGlobal.y)
    const zone = overlay.hitTest(pOverlay)
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
      // Only float panel if it is not already floating
      if (!panel.currentGroup || !panel.currentGroup.isFloating)
        floatPanel(panel, posGlobal)
    }
  }


  /**
    * Registers a drop target (adds to the extra target array).
    */
  function registerDropTarget(t) {
    if (t && extraDropTargets.indexOf(t) < 0)
      extraDropTargets.push(t)
  }


  /**
    * Unregisters a drop target (removes from the extra target array)
    */
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

  // Overlay shown during drags that displays the various zones
  DockOverlay {
    id: overlay
  }
}
