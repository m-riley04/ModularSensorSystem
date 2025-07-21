pragma Singleton

import QtQuick
import QtQuick.Controls

Item {
    id: globalActions

    // Action groups
    property alias groupFile: actionsFile
    property alias groupEdit: actionsEdit
    property alias groupView: actionsView
    property alias groupViewControlRow: actionsViewControlRow
    property alias groupAbout: actionsAbout
    property alias groupHelp: actionsHelp

    // Actions
    property alias quit: actionQuit
    property alias restart: actionRestart
    property alias viewMenuBar: actionViewMenuBar
    property alias viewToolbar: actionViewToolbar
    property alias viewCustomWindowHandle: actionViewCustomWindowHandle
    property alias viewControlRowPresetList: actionViewControlRowPresetList
    property alias viewControlRowSourcesList: actionViewControlRowSourcesList
    property alias viewControlRowProcessorsList: actionViewControlRowProcessorsList
    property alias viewControlRowControls: actionViewControlRowControls
    property alias viewControlRowEntireRow: actionViewControlRowEntireRow
    property alias aboutGitHub: actionAboutGitHub
    property alias aboutCredits: actionAboutCredits
    property alias help: actionHelp

    readonly property string githubUrl: "https://github.com/m-riley04/ModularSensorSystem"

    // Add actions
    ActionGroup {
        id: actionsFile
        Action {
            id: actionQuit
            text: qsTr("Quit")
            onTriggered: Qt.quit()
        }
        Action {
            id: actionRestart
            text: qsTr("Restart")
            onTriggered: {

            }
        }
    }

    ActionGroup {
        id: actionsEdit
    }

    ActionGroup {
        id: actionsView
        Action {
            id: actionViewMenuBar
            text: qsTr("Menu Bar")
            checkable: true
        }
        Action {
            id: actionViewToolbar
            text: qsTr("Toolbar")
            checkable: true
        }
        Action {
            id: actionViewCustomWindowHandle
            text: qsTr("Custom Window Handle")
            checkable: true
        }
    }

    ActionGroup {
        id: actionsViewControlRow
        Action {
            id: actionViewControlRowPresetList
            text: qsTr("Preset List")
            checkable: true
        }
        Action {
            id: actionViewControlRowSourcesList
            text: qsTr("Sources List")
            checkable: true
        }
        Action {
            id: actionViewControlRowProcessorsList
            text: qsTr("Processors List")
            checkable: true
        }
        Action {
            id: actionViewControlRowControls
            text: qsTr("Controls")
            checkable: true
        }
        Action {
            id: actionViewControlRowEntireRow
            text: qsTr("Entire Row")
            checkable: true
        }
    }

    ActionGroup {
        id: actionsAbout
        Action {
            id: actionAboutGitHub
            text: qsTr("GitHub")
            onTriggered: () => {
                Qt.openUrlExternally(globalActions.githubUrl)
            }
        }
        Action {
            id: actionAboutCredits
            text: qsTr("Credits")
        }
    }

    ActionGroup {
        id: actionsHelp
        Action {
            id: actionHelp
            text: qsTr("Help")
        }
    }
}
