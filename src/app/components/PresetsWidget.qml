import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

RowLayout {
  id: presetsWidget

  property alias model: presetsList.model
  property int selectedIndex: presetsList.currentIndex
  signal saveClicked
  signal loadClicked
  signal removeClicked
  signal refreshClicked
  signal selectedPresetChanged(int currentIndex, int previousIndex)
  spacing: 6
  Layout.fillWidth: true
  Layout.fillHeight: true

  // Item list
  ListView {
    id: presetsList
    Layout.minimumWidth: 100
    Layout.fillWidth: true
    Layout.fillHeight: true
    model: ListModel {
      ListElement {
        name: qsTr("hi")
      }
    }
    delegate: ItemDelegate {
      required property string name

      id: delegate
      width: ListView.view.width
      text: name
      onClicked: {

        // let prev = presetsList.currentIndex;
        // presetsList.currentIndex = index;
        // presetsWidget.selectedPresetChanged(index, prev);
      }
      highlighted: ListView.isCurrentItem
    }
    onCurrentIndexChanged: {
      let prev = -1
      // QML doesn't track previous, so -1
      presetsWidget.selectedPresetChanged(currentIndex, prev)
    }
  }

  // tool/control buttons
  ColumnLayout {
    id: controlsColumn
    spacing: 6
    Layout.preferredWidth: 48
    ToolButton {
      icon.name: "document-save"
      ToolTip.text: qsTr("Save a new preset")
      onClicked: presetsWidget.saveClicked()
    }
    ToolButton {
      icon.name: "document-open"
      ToolTip.text: qsTr("Load the selected preset")
      enabled: presetsList.currentIndex >= 0
      onClicked: presetsWidget.loadClicked()
    }
    ToolButton {
      icon.name: "edit-delete"
      ToolTip.text: qsTr("Delete the selected preset")
      enabled: presetsList.currentIndex >= 0
      onClicked: presetsWidget.removeClicked()
    }
    ToolButton {
      icon.name: "view-refresh"
      ToolTip.text: qsTr("Refresh the presets list")
      onClicked: presetsWidget.refreshClicked()
    }
    Item {
      Layout.fillHeight: true
    }
  }
}
