import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import app.components

SplitView {
    id: mainPage
    anchors.fill: parent
    orientation: Qt.Vertical

    // Top section
    SourcePreviewWidget {}
    // Bottom section
    RowLayout {
        spacing: 8
        // Presets group
        GroupBox {
            Layout.fillHeight: true
            title: qsTr("Presets")
            Layout.fillWidth: true
            Layout.preferredHeight: 100
            PresetsWidget {
                Layout.fillWidth: true
            }
        }
        // Processors group
        GroupBox {
            title: qsTr("Processors")
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.preferredHeight: 100
            ProcessorsWidget {
                Layout.fillWidth: true
            }
        }
        // Sources group
        GroupBox {
            Layout.fillHeight: true
            title: qsTr("Sources")
            Layout.fillWidth: true
            Layout.preferredHeight: 100
            SourcesWidget {
                Layout.fillWidth: true
            }
        }
        // Controls group
        GroupBox {
            Layout.fillHeight: true
            title: qsTr("Controls")
            Layout.fillWidth: true
            Layout.preferredHeight: 100
            SessionControlsWidget {
                Layout.fillWidth: true
            }
        }
    }
}
