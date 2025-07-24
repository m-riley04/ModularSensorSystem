import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import app.logic.actions

ToolBar {
  id: homeToolBar
  RowLayout {
    anchors.fill: parent
    spacing: 4
    ToolButton {
      action: PresetsActions.savePreset
      text: ""
    }
    ToolButton {
      action: PresetsActions.loadPreset
      text: ""
    }
    ToolButton {
      action: PresetsActions.refreshPresets
      text: ""
    }
    ToolButton {
      action: PresetsActions.deletePreset
      text: ""
    }
    ToolSeparator {}
    ToolButton {
      action: SourcesActions.addSource
      text: ""
    }
    ToolButton {
      action: SourcesActions.removeSource
      text: ""
    }
    ToolButton {
      action: SourcesActions.configureSource
      text: ""
    }
    ToolSeparator {}
    ToolButton {
      action: ProcessorsActions.addProcessor
      text: ""
    }
    ToolButton {
      action: ProcessorsActions.removeProcessor
      text: ""
    }
    ToolButton {
      action: ProcessorsActions.configureProcessor
      text: ""
    }
    ToolButton {
      action: ProcessorsActions.toggleProcessing
      text: ""
    }
  }
}
