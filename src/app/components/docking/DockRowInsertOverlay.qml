import QtQuick


/**
  * The insert bar overlay to indicate where the panel will be placed.
  */
Rectangle {
  id: insertBar

  required property int dropIndex
  required property int panelCount
  required property var row

  width: 4
  radius: 2
  color: "#009dff"
  visible: dropIndex >= 0
  anchors.top: parent.top
  anchors.bottom: parent.bottom
  z: 1000
  x: {
    if (dropIndex < 0 || dropIndex > panelCount)
      return 0
    if (dropIndex === panelCount) {
      if (panelCount === 0)
        return 0
      const lastItem = row.children[row.children.length - 1]
      return lastItem ? lastItem.x + lastItem.width : 0
    }
    // map model index to the visual child at the same position
    const child = row.children[dropIndex]
    return child ? child.x : 0
  }
}
