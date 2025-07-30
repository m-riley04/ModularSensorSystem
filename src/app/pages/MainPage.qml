import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import app.components
import app.components.docking
import app.components.plugins

SplitView {
  id: mainPage
  anchors.fill: parent
  orientation: Qt.Vertical

  // Top section
  SourcePreviewWidget {
    id: sourcePreview
    SplitView.preferredHeight: mainPage.height * 0.65

  }
  // Bottom section
  RowLayout {
    id: bottomRow
    SplitView.preferredHeight: mainPage.height * 0.35
    spacing: 8

    // Docking root
    DockRoot {
      id: dockRoot
      extraDropTargets: [dock]
    }

    // Docking row
    DockRow {
      id: dock
      Layout.fillWidth: true
      Layout.fillHeight: true
      dockRoot: dockRoot

      // Presets group
      DockPanel {
        id: panelPresets
        title: qsTr("Presets")
        canUndock: false
        PresetsWidget {
          Layout.fillWidth: true
        }
      }
      // Processors
      DockPanel {
        id: panelProcessors
        title: qsTr("Processors")
        canUndock: false
        ProcessorsWidget {
          Layout.fillWidth: true
        }
      }
      // Sources
      DockPanel {
        id: panelSources
        title: qsTr("Sources")
        canUndock: false
        SourcesWidget {
          Layout.fillWidth: true
        }
      }
      // Session Controls
      DockPanel {
        id: panelSessionControls
        title: qsTr("Session Controls")
        canUndock: false
        SessionControlsWidget {
          Layout.fillWidth: true
        }
      }
    }
  }
}
