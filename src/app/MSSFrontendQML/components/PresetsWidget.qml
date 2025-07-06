import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: presetsWidget
    property alias model: presetsList.model
    property int selectedIndex: presetsList.currentIndex
    signal saveClicked()
    signal loadClicked()
    signal removeClicked()
    signal refreshClicked()
    signal selectedPresetChanged(int currentIndex, int previousIndex)

    RowLayout {
        anchors.fill: parent
        spacing: 6
        // Presets list
        ListView {
            id: presetsList
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            model: ListModel {}
            delegate: ItemDelegate {
                width: ListView.view.width
                text: model.name
                onClicked: {
                    let prev = presetsList.currentIndex;
                    presetsList.currentIndex = index;
                    presetsWidget.selectedPresetChanged(index, prev);
                }
                highlighted: ListView.isCurrentItem
            }
            onCurrentIndexChanged: {
                let prev = -1; // QML doesn't track previous, so -1
                presetsWidget.selectedPresetChanged(currentIndex, prev);
            }
        }
        // Controls
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
            Item { Layout.fillHeight: true } // Spacer
        }
    }
}