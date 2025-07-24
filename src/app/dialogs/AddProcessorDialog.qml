import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Dialog {
  id: addProcessorDialog
  property alias processorModel: processorComboBox.model
  property int selectedIndex: processorComboBox.currentIndex
  signal processorSelected(int index)
  signal processorConfirmed(int index)
  signal refreshClicked

  title: qsTr("Add Processor")
  standardButtons: Dialog.Ok | Dialog.Cancel
  modal: true
  width: 400
  height: 120

  ColumnLayout {
    anchors.fill: parent
    spacing: 10
    RowLayout {
      spacing: 8
      Label {
        text: qsTr("Processor")
      }
      ComboBox {
        id: processorComboBox
        Layout.fillWidth: true
        model: ListModel {}
        textRole: "name"
        onCurrentIndexChanged: addProcessorDialog.processorSelected(
                                 currentIndex)
      }
      ToolButton {
        icon.name: "view-refresh"
        ToolTip.text: qsTr("Refresh processor list")
        onClicked: addProcessorDialog.refreshClicked()
      }
    }
  }
  onAccepted: addProcessorDialog.processorConfirmed(
                processorComboBox.currentIndex)
}
