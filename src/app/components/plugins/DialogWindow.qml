import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window

Window {
  id: root

  property alias buttons: buttonBox.standardButtons
  default property alias content: contentRoot.data

  signal accepted
  signal rejected
  signal opened
  signal closed

  visible: false
  width: 900
  height: 600
  title: qsTr("Dialog")
  flags: Qt.Window

  ColumnLayout {
    width: root.width
    height: root.height

    Item {
      id: contentRoot
      Layout.fillWidth: true
      Layout.fillHeight: true
    }

    DialogButtonBox {
      id: buttonBox
      Layout.fillWidth: true
      standardButtons: Dialog.Ok | Dialog.Cancel
      onAccepted: {
        root.accepted()
        root.close()
      }
      onRejected: {
        root.rejected()
        root.close()
      }
    }
  }
}
