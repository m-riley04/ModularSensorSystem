import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts

Window {
    visible: true
    width: 900
    height: 600
    title: "ModularSensorSystem"

    // Custom Title Bar (placeholder, can be replaced with a custom QML component)
    Rectangle {
        id: titleBar
        color: "#282828"
        height: 32
        width: parent.width
        anchors.top: parent.top
        RowLayout {
            anchors.fill: parent
            spacing: 8
            Label {
                text: "ModularSensorSystem"
                color: "white"
                font.bold: true
                Layout.alignment: Qt.AlignVCenter
            }
            // Add window control buttons here if needed
        }
    }

    // Menu Bar
    MenuBar {
        id: menuBar
        anchors.top: titleBar.bottom
        width: parent.width
        Menu {
            title: qsTr("File")
            MenuItem { text: qsTr("Quit") }
            MenuItem { text: qsTr("Restart") }
        }
        Menu {
            title: qsTr("Edit")
        }
        Menu {
            title: qsTr("View")
            Menu {
                title: qsTr("Control Row")
                MenuItem { text: qsTr("Preset List") }
                MenuItem { text: qsTr("Sources List") }
                MenuItem { text: qsTr("Processors List") }
                MenuItem { text: qsTr("Controls") }
                MenuSeparator {}
                MenuItem { text: qsTr("Entire Row") }
            }
            MenuSeparator {}
            MenuItem { text: qsTr("Menu Bar") }
            MenuItem { text: qsTr("Toolbar") }
            MenuItem { text: qsTr("Custom Window Handle") }
        }
        Menu {
            title: qsTr("Session")
            Menu {
                title: qsTr("Sources")
                MenuItem { text: qsTr("Add Source") }
                MenuItem { text: qsTr("Remove Source") }
                MenuItem { text: qsTr("Configure Source") }
            }
            Menu {
                title: qsTr("Processors")
                MenuItem { text: qsTr("Add Processor") }
                MenuItem { text: qsTr("Remove Processor") }
                MenuItem { text: qsTr("Configure Processor") }
                MenuItem { text: qsTr("Toggle Processing") }
            }
            Menu {
                title: qsTr("Recording")
            }
            MenuItem { text: qsTr("Clipping") }
            MenuItem { text: qsTr("Options") }
        }
        Menu {
            title: qsTr("Plugins")
        }
        Menu {
            title: qsTr("About")
            MenuItem { text: qsTr("GitHub") }
            MenuItem { text: qsTr("Credits") }
        }
        Menu {
            title: qsTr("Help")
        }
    }

    // Toolbar
    ToolBar {
        id: toolBar
        anchors.top: menuBar.bottom
        width: parent.width
        RowLayout {
            anchors.fill: parent
            spacing: 4
            ToolButton { text: qsTr("Save Preset") }
            ToolButton { text: qsTr("Load Preset") }
            ToolButton { text: qsTr("Refresh Presets") }
            ToolButton { text: qsTr("Delete Preset") }
            ToolSeparator {}
            ToolButton { text: qsTr("Add Source") }
            ToolButton { text: qsTr("Remove Source") }
            ToolButton { text: qsTr("Configure Source") }
            ToolSeparator {}
            ToolButton { text: qsTr("Add Processor") }
            ToolButton { text: qsTr("Remove Processor") }
            ToolButton { text: qsTr("Configure Processor") }
            ToolButton { text: qsTr("Toggle Processing") }
        }
    }

    // Navigation Bar
    RowLayout {
        id: navBar
        anchors.top: toolBar.bottom
        width: parent.width
        height: 40
        spacing: 8
        Rectangle {
            color: "transparent"
            Layout.fillWidth: true
            Layout.preferredHeight: 40
            RowLayout {
                anchors.fill: parent
                spacing: 8
                Button { text: qsTr("Home") }
                Button { text: qsTr("Playback") }
                Item { Layout.fillWidth: true } // Spacer
            }
        }
    }

    // Main Page Stack
    StackView {
        id: pagesStack
        anchors.top: navBar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        initialItem: mainPageComponent
        Component {
            id: mainPageComponent
            Loader { source: "qrc:/qt/qml/mssfrontendqml/pages/MainPage.qml" }
        }
    }
}
