import QtQuick
import QtQuick.Controls
import app.logic.actions

MenuBar {
    id: menuBar

    GlobalActions {
        id: globalActions
    }
    SessionActions {
        id: sessionActions
    }
    SourcesActions {
        id: sourcesActions
    }
    PluginsActions {
        id: pluginsActions
    }
    ProcessorsActions {
        id: processorsActions
    }
    PresetsActions {
        id: presetsActions
    }

    width: parent.width

    Menu {
        title: qsTr("File")
        MenuItem {
            action: globalActions.quit
        }
        MenuItem {
            action: globalActions.restart
        }
    }
    Menu {
        title: qsTr("Edit")
    }
    Menu {
        title: qsTr("View")
        Menu {
            title: qsTr("Control Row")
            MenuItem {
                action: globalActions.viewControlRowPresetList
            }
            MenuItem {
                action: globalActions.viewControlRowSourcesList
            }
            MenuItem {
                action: globalActions.viewControlRowProcessorsList
            }
            MenuItem {
                action: globalActions.viewControlRowControls
            }
            MenuSeparator {}
            MenuItem {
                action: globalActions.viewControlRowEntireRow
            }
        }
        MenuSeparator {}
        MenuItem {
            action: globalActions.viewMenuBar
        }
        MenuItem {
            action: globalActions.viewToolbar
        }
        MenuItem {
            action: globalActions.viewCustomWindowHandle
        }
    }
    Menu {
        title: qsTr("Session")
        Menu {
            title: qsTr("Sources")
            MenuItem {
                action: sourcesActions.addSource
            }
            MenuItem {
                action: sourcesActions.removeSource
            }
            MenuItem {
                action: sourcesActions.configureSource
            }
        }
        Menu {
            title: qsTr("Processors")
            MenuItem {
                action: processorsActions.addProcessor
            }
            MenuItem {
                action: processorsActions.removeProcessor
            }
            MenuItem {
                action: processorsActions.configureProcessor
            }
            MenuItem {
                action: processorsActions.toggleProcessing
            }
        }
        Menu {
            title: qsTr("Recording")
        }
        MenuItem {
            action: sessionActions.clipping
        }
        MenuItem {
            action: sessionActions.sessionOptions
        }
    }
    Menu {
        title: qsTr("Plugins")
    }
    Menu {
        title: qsTr("About")
        MenuItem {
            action: globalActions.aboutGitHub
        }
        MenuItem {
            action: globalActions.aboutCredits
        }
    }
    Menu {
        title: qsTr("Help")
        MenuItem {
            action: globalActions.help
        }
    }
}
