import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: processorsWidget
    property alias model: processorsList.model
    property int selectedIndex: processorsList.currentIndex
    property bool enabled: enableCheckBox.checked
    signal addClicked()
    signal removeClicked()
    signal propertiesClicked()
    signal selectedProcessorChanged(int currentIndex, int previousIndex)

    RowLayout {
        anchors.fill: parent
        spacing: 6
        // Processors list
        ListView {
            id: processorsList
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            model: ListModel {}
            delegate: ItemDelegate {
                width: ListView.view.width
                text: model.name
                onClicked: {
                    let prev = processorsList.currentIndex;
                    processorsList.currentIndex = index;
                    processorsWidget.selectedProcessorChanged(index, prev);
                }
                highlighted: ListView.isCurrentItem
            }
            onCurrentIndexChanged: {
                let prev = -1; // QML doesn't track previous, so -1
                processorsWidget.selectedProcessorChanged(currentIndex, prev);
            }
        }
        // Controls
        ColumnLayout {
            id: controlsColumn
            spacing: 6
            Layout.preferredWidth: 48
            ToolButton {
                icon.name: "list-add"
                ToolTip.text: qsTr("Add a new processor")
                onClicked: processorsWidget.addClicked()
            }
            ToolButton {
                icon.name: "list-remove"
                ToolTip.text: qsTr("Remove the selected processor")
                enabled: processorsList.currentIndex >= 0
                onClicked: processorsWidget.removeClicked()
            }
            ToolButton {
                icon.name: "document-properties"
                ToolTip.text: qsTr("Open properties for the selected processor")
                enabled: processorsList.currentIndex >= 0
                onClicked: processorsWidget.propertiesClicked()
            }
            CheckBox {
                id: enableCheckBox
                text: qsTr("Enabled")
                checked: true
                onCheckedChanged: processorsWidget.enabledChanged(checked)
            }
            Item { Layout.fillHeight: true } // Spacer
        }
    }
}