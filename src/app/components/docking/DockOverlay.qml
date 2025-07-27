import QtQuick

Item {
  id: root
  anchors.fill: parent
  visible: false
  z: 99999
  property int targetZone: Docking.DockZone.NONE

  // Visual target zones
  Rectangle {
    id: centerZone
    color: Qt.rgba(0, 0.6, 1,
                   root.targetZone === Docking.DockZone.CENTER ? 0.35 : 0.15)
    anchors.centerIn: parent
    width: parent.width * 0.4
    height: parent.height * 0.4
    radius: 8
    border.color: Qt.rgba(0, 0.6, 1, 0.5)
  }
  Rectangle {
    id: leftZone
    color: Qt.rgba(0, 0.6, 1,
                   root.targetZone === Docking.DockZone.LEFT ? 0.35 : 0.15)
    anchors.top: parent.top
    anchors.bottom: parent.bottom
    anchors.left: parent.left
    width: Math.min(200, parent.width * 0.23)
    border.color: Qt.rgba(0, 0.6, 1, 0.5)
  }
  Rectangle {
    id: rightZone
    color: Qt.rgba(0, 0.6, 1,
                   root.targetZone === Docking.DockZone.RIGHT ? 0.35 : 0.15)
    anchors.top: parent.top
    anchors.bottom: parent.bottom
    anchors.right: parent.right
    width: Math.min(200, parent.width * 0.23)
    border.color: Qt.rgba(0, 0.6, 1, 0.5)
  }
  Rectangle {
    id: topZone
    color: Qt.rgba(0, 0.6, 1,
                   root.targetZone === Docking.DockZone.TOP ? 0.35 : 0.15)
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.top: parent.top
    height: Math.min(160, parent.height * 0.20)
    border.color: Qt.rgba(0, 0.6, 1, 0.5)
  }
  Rectangle {
    id: bottomZone
    color: Qt.rgba(0, 0.6, 1,
                   root.targetZone === Docking.DockZone.BOTTOM ? 0.35 : 0.15)
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.bottom: parent.bottom
    height: Math.min(160, parent.height * 0.20)
    border.color: Qt.rgba(0, 0.6, 1, 0.5)
  }

  Rectangle {
    anchors.fill: parent
    color: "transparent"
    border.color: Qt.rgba(1, 1, 1, 0.05)
  }

  function inItem(item, pScene) {
    const lp = item.mapFromItem(null, pScene)
    // convert to item-local coords
    return item.contains(lp) // Item.contains() expects local coords
  }

  function hitTest(p): string {
    if (!visible)
      return Docking.DockZone.NONE
    if (inItem(leftZone, p))
      return Docking.DockZone.LEFT
    if (inItem(rightZone, p))
      return Docking.DockZone.RIGHT
    if (inItem(topZone, p))
      return Docking.DockZone.TOP
    if (inItem(bottomZone, p))
      return Docking.DockZone.BOTTOM
    if (inItem(centerZone, p))
      return Docking.DockZone.CENTER
    return Docking.DockZone.NONE
  }
}
