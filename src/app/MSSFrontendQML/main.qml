import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import "pages"
import "components"
import "logic/actions"

Window {
    visible: true
    width: 900
    height: 600
    title: "ModularSensorSystem"

    GlobalActions { id: globalActions }
    SessionActions { id: sessionActions }
    SourcesActions { id: sourcesActions }
    PluginsActions { id: pluginsActions }
    ProcessorsActions { id: processorsActions }
    PresetsActions { id: presetsActions }

    // Custom Title Bar (placeholder, can be replaced with a custom QML component)
    CustomTitleBar {
        id: titleBar
    }

    // Menu Bar
    MenuBar {
        id: menuBar
        anchors.top: titleBar.bottom
        width: parent.width
        Menu {
            title: qsTr("File")
            MenuItem { action: globalActions.quit }
            MenuItem { action: globalActions.restart }
        }
        Menu {
            title: qsTr("Edit")
        }
        Menu {
            title: qsTr("View")
            Menu {
                title: qsTr("Control Row")
                MenuItem { action: globalActions.viewControlRowPresetList }
                MenuItem { action: globalActions.viewControlRowSourcesList }
                MenuItem { action: globalActions.viewControlRowProcessorsList }
                MenuItem { action: globalActions.viewControlRowControls }
                MenuSeparator {}
                MenuItem { action: globalActions.viewControlRowEntireRow }
            }
            MenuSeparator {}
            MenuItem { action: globalActions.viewMenuBar }
            MenuItem { action: globalActions.viewToolbar }
            MenuItem { action: globalActions.viewCustomWindowHandle }
        }
        Menu {
            title: qsTr("Session")
            Menu {
                title: qsTr("Sources")
                MenuItem { action: sourcesActions.addSource }
                MenuItem { action: sourcesActions.removeSource }
                MenuItem { action: sourcesActions.configureSource }
            }
            Menu {
                title: qsTr("Processors")
                MenuItem { action: processorsActions.addProcessor }
                MenuItem { action: processorsActions.removeProcessor }
                MenuItem { action: processorsActions.configureProcessor }
                MenuItem { action: processorsActions.toggleProcessing }
            }
            Menu {
                title: qsTr("Recording")
            }
            MenuItem { action: sessionActions.clipping }
            MenuItem { action: sessionActions.sessionOptions }
        }
        Menu {
            title: qsTr("Plugins")
        }
        Menu {
            title: qsTr("About")
            MenuItem { action: globalActions.aboutGitHub }
            MenuItem { action: globalActions.aboutCredits }
        }
        Menu {
            title: qsTr("Help")
            MenuItem { action: globalActions.help }
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
            ToolButton { action: presetsActions.savePreset; }
            ToolButton { action: presetsActions.loadPreset }
            ToolButton { action: presetsActions.refreshPresets }
            ToolButton { action: presetsActions.deletePreset }
            ToolSeparator {}
            ToolButton { action: sourcesActions.addSource  }
            ToolButton { action: sourcesActions.removeSource }
            ToolButton { action: sourcesActions.configureSource }
            ToolSeparator {}
            ToolButton { action: processorsActions.addProcessor }
            ToolButton { action: processorsActions.removeProcessor }
            ToolButton { action: processorsActions.configureProcessor }
            ToolButton { action: processorsActions.toggleProcessing }
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
        MainPage {}
    }

    
}
