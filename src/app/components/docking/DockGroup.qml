import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
  id: root
  property var dockRoot
  // expose a count (StackLayout has count, but we use children length since we parent panels directly)
  readonly property int count: stack.children.length
  property bool isFloating: false

  // TabButton factory for each group
  Component {
    id: tabButtonComp
    TabButton {}
  }


  /**
    * Gets the index of a specific panel in the group.
    */
  function indexOf(panel) {
    return stack.children.indexOf(panel) // <- StackLayout manages its children
  }


  /**
    * Adds a panel to the group.
    */
  function addPanel(panel) {
    panel.dockRoot = dockRoot

    let idx = indexOf(panel)
    if (idx === -1) {
      // create + insert tab
      const tab = tabButtonComp.createObject(tabs, {
                                               "text": panel.title
                                             })
      tabs.addItem(tab) // TabBar is a Container
      // parent panel into the stack so StackLayout manages it
      panel.parent = stack
      panel.Layout.fillWidth = true
      panel.Layout.fillHeight = true
      // keep tab text synced
      if (panel.titleChanged)
        panel.titleChanged.connect(() => tab.text = panel.title)
      idx = indexOf(panel)
    }
    tabs.currentIndex = idx
    stack.currentIndex = idx
    panel.currentGroup = root
  }


  /**
    * Removes a panel from the group.
    */
  function removePanel(panel) {
    tabs.removeItem(panel)
    panel.parent = null
  }

  ColumnLayout {
    anchors.fill: parent
    spacing: 0

    TabBar {
      id: tabs
      Layout.fillWidth: true
      contentHeight: 34
      onCurrentIndexChanged: stack.currentIndex = currentIndex
    }

    StackLayout {
      id: stack
      Layout.fillWidth: true
      Layout.fillHeight: true
      // NOTE: StackLayout has count/currentIndex; children define the pages.
    }
  }
}
