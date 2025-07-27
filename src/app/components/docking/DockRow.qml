import QtQuick
import QtQuick.Layouts

Item {
  id: rowDock
  property var dockRoot
  // set by parent
  readonly property int count: row.children.length
  property bool isUnderDrag: false

  // === internal drag bookkeeping ===
  property var dragPanel: null
  property int dropIndex: -1 // where the blue bar shows

  signal panelAdded(DockPanel panel)

  // visual drop indicator
  Rectangle {
    id: dropBar
    width: 4
    radius: 2
    color: rowDock.dropIndex >= 0 ? "#009dff" : "transparent"
    height: parent.height
    visible: rowDock.dropIndex >= 0
    z: 10
    x: {
      // position bar just before the child at dropIndex
      if (rowDock.dropIndex < 0 || rowDock.dropIndex > row.children.length)
        return 0
      if (rowDock.dropIndex === row.children.length) {
        const last = row.children.length ? row.children[row.children.length - 1] : null
        return last ? last.x + last.width : 0
      }
      return row.children[rowDock.dropIndex].x
    }
  }

  RowLayout {
    id: row
    anchors.fill: parent
    spacing: 6
  }

  // === Public API ===
  function addPanel(panel, index) {
    if (!panel)
      return
    panel.dockRoot = dockRoot
    panel.parent = row
    panel.Layout.fillHeight = true
    panel.Layout.preferredWidth = 240 // tweak to taste
    panel.currentGroup = rowDock
    if (index !== undefined && index >= 0 && index <= row.children.length - 1)
      row.children.move(row.children.length - 1,
                        index) // re-order newly appended
    panelAdded(panel)
  }

  function removePanel(panel) {
    const i = row.children.indexOf(panel)
    if (i >= 0)
      panel.parent = null
  }

  // === drag lifecycle from DockPanel ===
  function beginDrag(panel) {
    dragPanel = panel
    dropIndex = -1
  }
  function updateDrag(pScene) {
    if (!dragPanel) {
      isUnderDrag = false
      dropIndex = -1
      return
    }
    // map point into row coordinates
    const local = row.mapFromItem(null, pScene)
    isUnderDrag = (local.x >= 0 && local.y >= 0 && local.x <= row.width
                   && local.y <= row.height)
    if (!isUnderDrag) {
      dropIndex = -1
      return
    }
    // decide which gap it's over
    let idx = 0
    for (; idx < row.children.length; ++idx) {
      if (local.x < row.children[idx].x + row.children[idx].width / 2)
        break
    }
    dropIndex = idx
  }
  function endDrag(panel, p/* overlay coords */ ) {
    if (panel !== dragPanel)
      return
    if (isUnderDrag && dropIndex >= 0) {
      addPanel(panel, dropIndex)
    } else {
      dockRoot.floatPanel(panel) // outside row → float
    }
    dragPanel = null
    dropIndex = -1
    isUnderDrag = false
  }
}
