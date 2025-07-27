import QtQuick
import QtQuick.Layouts
import QtQml.Models

Item {
  id: rowDock
  property var dockRoot
  readonly property int count: panelModel.count
  property bool isUnderDrag: false
  property int panelMinWidth: 160
  property int panelPrefWidth: 240
  property int panelMaxWidth: 16777215 // effectively "infinite"
  property bool equalStretch: true // share width evenly

  // Author DockPanel{} statically under DockRow:
  // they go into the ObjectModel, not directly into RowLayout.
  default property alias panels: panelModel.children

  // Hold the panels in a model we can reorder
  ObjectModel {
    id: panelModel
  }

  RowLayout {
    id: row
    anchors.fill: parent
    spacing: 6

    // Present the model; Repeater inserts items as children of RowLayout
    Repeater {
      id: rep
      model: panelModel
    }

    Connections {
      target: rep
      function onItemAdded(index, item) {
        if (item && item.hasOwnProperty("dockRoot")) {
          item.dockRoot = rowDock.dockRoot
          item.currentGroup = rowDock
          item.Layout.fillHeight = true
          item.Layout.fillWidth = equalStretch
          item.Layout.minimumWidth = panelMinWidth
          item.Layout.preferredWidth = panelPrefWidth
          item.Layout.maximumWidth = panelMaxWidth
        }
      }
    }
  }


  /**
    * Adds a panel to the row at a given index.
    */
  function addPanel(panel, index) {
    if (!panel)
      return
    if (index !== undefined && index >= 0 && index <= panelModel.count)
      panelModel.insert(index, panel)
    else
      panelModel.append(panel)
    panel.dockRoot = dockRoot
    panel.currentGroup = rowDock
    panel.Layout.fillHeight = true
    panel.Layout.fillWidth = equalStretch
    panel.Layout.minimumWidth = panelMinWidth
    panel.Layout.preferredWidth = panelPrefWidth
    panel.Layout.maximumWidth = panelMaxWidth
    const last = panelModel.count - 1
    if (index !== undefined && index >= 0 && index < panelModel.count)
      panelModel.move(last, index, 1)
  }


  /**
    * Removes a panel from the row.
    */
  function removePanel(panel) {
    // find index of this panel in the ObjectModel
    const kids = panelModel.children
    for (var i = 0; i < kids.length; ++i) {
      if (kids[i] === panel) {
        panelModel.remove(i, 1)
        break
      }
    }
  }

  // === drag bookkeeping ===
  property var dragPanel: null
  property int dropIndex: -1

  DockRowInsertOverlay {
    id: dropBar
    row: row
    dropIndex: rowDock.dropIndex
    panelCount: panelModel.count
    height: parent.height
  }


  /**
    * Called when beginning to drag a panel.
    */
  function beginDrag(panel) {
    dragPanel = panel
    dropIndex = -1
  }


  /**
    * A handler that is called when a drag event happens.
    */
  function updateDrag(posGlobal) {
    if (!dragPanel) {
      isUnderDrag = false
      dropIndex = -1
      return
    }
    const local = row.mapFromGlobal(posGlobal.x, posGlobal.y)
    isUnderDrag = (local.x >= 0 && local.y >= 0 && local.x <= row.width
                   && local.y <= row.height)
    if (!isUnderDrag) {
      dropIndex = -1
      return
    }

    // decide gap index against *visual* children produced by Repeater
    const kids = row.children
    let idx = 0
    for (; idx < kids.length; ++idx) {
      const it = kids[idx]
      if (!it || !it.hasOwnProperty("dockRoot"))
        continue
      if (local.x < it.x + it.width / 2)
        break
    }
    // clamp to [0 .. panelModel.count]
    dropIndex = Math.max(0, Math.min(idx, panelModel.count))
  }


  /**
    * Called when a drop event occurs (a.k.a. drag event ends)
    */
  function endDrag(panel, posScene) {
    if (panel !== dragPanel)
      return
    if (isUnderDrag && dropIndex >= 0) {
      // find 'from' index of this panel in the model
      let from = -1
      const kids = panelModel.children
      for (var i = 0; i < kids.length; ++i)
        if (kids[i] === panel) {
          from = i
          break
        }
      if (from === -1) {
        // not in model yet → append then move into place
        addPanel(panel, dropIndex)
      } else if (from !== dropIndex && dropIndex <= panelModel.count) {
        // move within model
        panelModel.move(from, dropIndex > from ? dropIndex - 1 : dropIndex, 1)
      }
    } else {
      dockRoot.floatPanel(panel) // outside row → float
    }
    dragPanel = null
    dropIndex = -1
    isUnderDrag = false
  }
}
