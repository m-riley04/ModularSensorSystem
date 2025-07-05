import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: mainPage
    anchors.fill: parent

    SplitView {
        id: mainSplitter
        orientation: Qt.Vertical
        anchors.fill: parent

        // Top section: Views
        Rectangle {
            color: "#2d2d2d"
            implicitHeight: 300
            anchors.left: parent.left
            anchors.right: parent.right
            GridLayout {
                anchors.fill: parent
                columns: 2
                // Device Preview Widget placeholder
                Rectangle {
                    color: "#444"
                    Layout.row: 0
                    Layout.column: 0
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Label {
                        text: "DevicePreviewWidget"
                        anchors.centerIn: parent
                        color: "white"
                    }
                }
                // Controls frame
                Rectangle {
                    color: "#333"
                    Layout.row: 0
                    Layout.column: 1
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    ColumnLayout {
                        anchors.fill: parent
                        spacing: 8
                        // Presets group
                        GroupBox {
                            title: qsTr("Presets")
                            Layout.fillWidth: true
                            Layout.preferredHeight: 100
                            Label { text: "PresetsWidget"; anchors.centerIn: parent }
                        }
                        // Processors group
                        GroupBox {
                            title: qsTr("Processors")
                            Layout.fillWidth: true
                            Layout.preferredHeight: 100
                            Label { text: "ProcessorsWidget"; anchors.centerIn: parent }
                        }
                        // Sources group
                        GroupBox {
                            title: qsTr("Sources")
                            Layout.fillWidth: true
                            Layout.preferredHeight: 100
                            Label { text: "SourcesWidget"; anchors.centerIn: parent }
                        }
                        // Controls group
                        GroupBox {
                            title: qsTr("Controls")
                            Layout.fillWidth: true
                            Layout.preferredHeight: 100
                            Label { text: "SessionControlsWidget"; anchors.centerIn: parent }
                        }
                    }
                }
            }
        }
        // Bottom section: Dockable elements (placeholder)
        Rectangle {
            color: "#232323"
            implicitHeight: 100
            anchors.left: parent.left
            anchors.right: parent.right
            Label {
                text: "DockableElementsManagerWidget"
                anchors.centerIn: parent
                color: "white"
            }
        }
    }
}
