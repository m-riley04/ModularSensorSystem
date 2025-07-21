import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import app.logic.actions

// Toolbar
ToolBar {
    id: homeToolBar

    SourcesActions {
        id: sourcesActions
    }
    ProcessorsActions {
        id: processorsActions
    }
    PresetsActions {
        id: presetsActions
    }

    RowLayout {
        anchors.fill: parent
        spacing: 4
        ToolButton {
            action: presetsActions.savePreset
            text: ""
        }
        ToolButton {
            action: presetsActions.loadPreset
            text: ""
        }
        ToolButton {
            action: presetsActions.refreshPresets
            text: ""
        }
        ToolButton {
            action: presetsActions.deletePreset
            text: ""
        }
        ToolSeparator {}
        ToolButton {
            action: sourcesActions.addSource
            text: ""
        }
        ToolButton {
            action: sourcesActions.removeSource
            text: ""
        }
        ToolButton {
            action: sourcesActions.configureSource
            text: ""
        }
        ToolSeparator {}
        ToolButton {
            action: processorsActions.addProcessor
            text: ""
        }
        ToolButton {
            action: processorsActions.removeProcessor
            text: ""
        }
        ToolButton {
            action: processorsActions.configureProcessor
            text: ""
        }
        ToolButton {
            action: processorsActions.toggleProcessing
            text: ""
        }
    }
}
