import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
  id: customTitleBar
  color: "#282828"
  height: 32
  Layout.preferredWidth: 600
  width: 600
  property alias title: titleLabel.text
  property alias iconSource: iconLabel.source

  RowLayout {
    anchors.fill: parent
    spacing: 5
    // Icon
    Image {
      id: iconLabel
      objectName: "titleIcon"
      source: "" // Set icon source as needed
      fillMode: Image.PreserveAspectFit
      visible: source && source.toString().length > 0
    }
    // Title
    Label {
      id: titleLabel
      objectName: "titleLabel"
      text: qsTr("Title")
      color: "white"
      font.bold: true
      verticalAlignment: Label.AlignVCenter
    }
    Item {
      Layout.fillWidth: true
    } // Spacer
    // Window control buttons
    RowLayout {
      spacing: 0
      ToolButton {
        id: minimizeButton
        objectName: "minimizeButton"
        icon.name: "list-remove"
        onClicked: customTitleBar.Window.window ? customTitleBar.Window.window.showMinimized(
                                                    ) : undefined
        ToolTip.text: qsTr("Minimize")
      }
      ToolButton {
        id: maximizeButton
        objectName: "maximizeButton"
        icon.name: customTitleBar.Window.window
                   && customTitleBar.Window.window.visibility
                   === Window.Maximized ? "view-fullscreen" : "view-fullscreen"
        onClicked: {
          if (customTitleBar.Window.window) {
            if (customTitleBar.Window.window.visibility === Window.Maximized)
              customTitleBar.Window.window.showNormal()
            else
              customTitleBar.Window.window.showMaximized()
          }
        }
        ToolTip.text: qsTr("Maximize/Restore")
      }
      ToolButton {
        id: closeButton
        objectName: "closeButton"
        icon.name: "window-close"
        onClicked: customTitleBar.Window.window ? customTitleBar.Window.window.close(
                                                    ) : undefined
        ToolTip.text: qsTr("Close")
      }
    }
  }
  // Drag to move window
  DragHandler {
    id: dragHandler
    target: null
    onActiveChanged: if (active && customTitleBar.Window.window)
                       customTitleBar.Window.window.startSystemMove()
  }
}
