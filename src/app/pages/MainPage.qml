import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import app.components

Rectangle {
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
            // Source Preview Widget placeholder
            SourcePreviewWidget {
                Layout.row: 0
                Layout.column: 0
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }
        // Bottom section: Dockable control elements (placeholder)
        Rectangle {
            color: "#232323"
            implicitHeight: 100
            anchors.left: parent.left
            anchors.right: parent.right
            RowLayout {
                anchors.fill: parent
                spacing: 8
                // Presets group
                GroupBox {
                    title: qsTr("Presets")
                    Layout.fillWidth: true
                    Layout.preferredHeight: 100
                    PresetsWidget {
                        anchors.centerIn: parent
                        Layout.fillWidth: true
                    }
                }
                // Processors group
                GroupBox {
                    title: qsTr("Processors")
                    Layout.fillWidth: true
                    Layout.preferredHeight: 100
                    ProcessorsWidget {
                        anchors.centerIn: parent
                        Layout.fillWidth: true
                    }
                }
                // Sources group
                GroupBox {
                    title: qsTr("Sources")
                    Layout.fillWidth: true
                    Layout.preferredHeight: 100
                    SourcesWidget {
                        anchors.centerIn: parent
                        Layout.fillWidth: true
                    }
                }
                // Controls group
                GroupBox {
                    title: qsTr("Controls")
                    Layout.fillWidth: true
                    Layout.preferredHeight: 100
                    SessionControlsWidget {
                        anchors.centerIn: parent
                        Layout.fillWidth: true
                    }
                }
            }
        }
    }
}
