import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import App.Components
import App.Pages
import App.Logic.Actions

Window {
    id: window
    visible: true
    width: 900
    height: 600
    title: "ModularSensorSystem"
    palette.window: 'gray'
    flags: Qt.Window | Qt.FramelessWindowHint

    GlobalActions { id: globalActions }
    SessionActions { id: sessionActions }
    SourcesActions { id: sourcesActions }
    PluginsActions { id: pluginsActions }
    ProcessorsActions { id: processorsActions }
    PresetsActions { id: presetsActions }

    // Custom Title Bar (placeholder, can be replaced with a custom QML component)
    CustomTitleBar {
        id: titleBar
        title: window.title
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
            ToolButton { action: presetsActions.savePreset; text: ""; }
            ToolButton { action: presetsActions.loadPreset; text: ""; }
            ToolButton { action: presetsActions.refreshPresets; text: ""; }
            ToolButton { action: presetsActions.deletePreset; text: ""; }
            ToolSeparator {}
            ToolButton { action: sourcesActions.addSource; text: ""; }
            ToolButton { action: sourcesActions.removeSource; text: ""; }
            ToolButton { action: sourcesActions.configureSource; text: ""; }
            ToolSeparator {}
            ToolButton { action: processorsActions.addProcessor; text: ""; }
            ToolButton { action: processorsActions.removeProcessor; text: ""; }
            ToolButton { action: processorsActions.configureProcessor; text: ""; }
            ToolButton { action: processorsActions.toggleProcessing; text: ""; }
        }
    }

    // Main Page Stack
    StackView {
        id: pagesStack
        anchors.top: toolBar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        MainPage {}
    }

}
