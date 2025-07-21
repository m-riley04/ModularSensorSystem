import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Dialog {
    id: addSourceDialog
    property alias sourceTypeModel: sourceTypeComboBox.model
    property alias sourceModel: sourceComboBox.model
    property int selectedSourceTypeIndex: sourceTypeComboBox.currentIndex
    property int selectedSourceIndex: sourceComboBox.currentIndex
    signal sourceTypeSelected(int index)
    signal sourceSelected(int index)
    signal sourceConfirmed(int sourceTypeIndex, int sourceIndex)
    signal refreshClicked

    title: qsTr("Add Source")
    standardButtons: Dialog.Ok | Dialog.Cancel
    modal: true
    width: 440
    height: 180

    ColumnLayout {
        anchors.fill: parent
        spacing: 10
        RowLayout {
            spacing: 8
            Label {
                text: qsTr("Type")
            }
            ComboBox {
                id: sourceTypeComboBox
                Layout.fillWidth: true
                model: ListModel {}
                textRole: "name"
                onCurrentIndexChanged: addSourceDialog.sourceTypeSelected(
                                           currentIndex)
            }
        }
        RowLayout {
            spacing: 8
            Label {
                text: qsTr("Source")
            }
            ComboBox {
                id: sourceComboBox
                Layout.fillWidth: true
                model: ListModel {}
                textRole: "displayName"
                onCurrentIndexChanged: addSourceDialog.sourceSelected(
                                           currentIndex)
            }
            ToolButton {
                icon.name: "view-refresh"
                ToolTip.text: qsTr("Refresh source list")
                onClicked: addSourceDialog.refreshClicked()
            }
        }
    }
    onAccepted: addSourceDialog.sourceConfirmed(
                    sourceTypeComboBox.currentIndex,
                    sourceComboBox.currentIndex)
}
