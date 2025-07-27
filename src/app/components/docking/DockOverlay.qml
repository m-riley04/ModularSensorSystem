import QtQuick

Item {
  id: root
  anchors.fill: parent
  visible: false
  z: 99999
  property int targetZone: Docking.DockZone.NONE

  // Visual target zones
  DockZoneOverlay {
    id: centerZone
    isHighlighted: root.targetZone === Docking.DockZone.CENTER
    anchors.centerIn: parent
    width: parent.width * 0.4
    height: parent.height * 0.4
  }

  DockZoneOverlay {
    id: leftZone
    isHighlighted: root.targetZone === Docking.DockZone.LEFT
    anchors.top: parent.top
    anchors.bottom: parent.bottom
    anchors.left: parent.left
    width: Math.min(200, parent.width * 0.23)
  }

  DockZoneOverlay {
    id: rightZone
    isHighlighted: root.targetZone === Docking.DockZone.RIGHT
    anchors.top: parent.top
    anchors.bottom: parent.bottom
    anchors.right: parent.right
    width: Math.min(200, parent.width * 0.23)
  }

  DockZoneOverlay {
    id: topZone
    isHighlighted: root.targetZone === Docking.DockZone.TOP
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.top: parent.top
    height: Math.min(160, parent.height * 0.20)
  }

  DockZoneOverlay {
    id: bottomZone
    isHighlighted: root.targetZone === Docking.DockZone.BOTTOM
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.bottom: parent.bottom
    height: Math.min(160, parent.height * 0.20)
  }

  Rectangle {
    anchors.fill: parent
    color: "transparent"
    border.color: Qt.rgba(1, 1, 1, 0.05)
  }


  /**
    * Checks if an item is within
    */
  function inItem(item, posScene) {
    const lp = item.mapFromItem(null, posScene)
    // convert to item-local coords
    return item.contains(lp) // Item.contains() expects local coords
  }


  /**
    * Tests if a position is within any of the zones.
    * @returns The zone which the position is in as a DockZone.
    */
  function hitTest(pos): int {
    if (!visible)
      return Docking.DockZone.NONE
    if (inItem(leftZone, pos))
      return Docking.DockZone.LEFT
    if (inItem(rightZone, pos))
      return Docking.DockZone.RIGHT
    if (inItem(topZone, pos))
      return Docking.DockZone.TOP
    if (inItem(bottomZone, pos))
      return Docking.DockZone.BOTTOM
    if (inItem(centerZone, pos))
      return Docking.DockZone.CENTER
    return Docking.DockZone.NONE
  }
}
