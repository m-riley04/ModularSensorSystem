import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

RowLayout {
  id: sourcesWidget
  property alias model: sourcesList.model
  property int selectedIndex: sourcesList.currentIndex
  signal addClicked
  signal removeClicked
  signal propertiesClicked
  signal selectedSourceChanged(int currentIndex, int previousIndex)

  spacing: 6
  Layout.fillWidth: true
  Layout.fillHeight: true
  ListView {
    id: sourcesList
    Layout.minimumWidth: 32
    Layout.fillWidth: true
    Layout.fillHeight: true
    model: ListModel {}
    delegate: ItemDelegate {
      width: ListView.view.width
      text: qsTr("model.name") //model.name
      onClicked: {

        // let prev = sourcesList.currentIndex
        // sourcesList.currentIndex = index
        // sourcesWidget.selectedSourceChanged(index, prev)
      }
      highlighted: ListView.isCurrentItem
    }
    onCurrentIndexChanged: {
      let prev = -1
      // QML doesn't track previous, so -1
      sourcesWidget.selectedSourceChanged(currentIndex, prev)
    }
  }
  // Controls
  ColumnLayout {
    id: controlsColumn
    spacing: 6
    Layout.preferredWidth: 48
    ToolButton {
      icon.name: "list-add"
      ToolTip.text: qsTr("Add a new source")
      onClicked: sourcesWidget.addClicked()
    }
    ToolButton {
      icon.name: "list-remove"
      ToolTip.text: qsTr("Remove the selected source")
      enabled: sourcesList.currentIndex >= 0
      onClicked: sourcesWidget.removeClicked()
    }
    ToolButton {
      icon.name: "document-properties"
      ToolTip.text: qsTr("Open properties for the selected source")
      enabled: sourcesList.currentIndex >= 0
      onClicked: sourcesWidget.propertiesClicked()
    }
    Item {
      Layout.fillHeight: true
    }
  }
}
