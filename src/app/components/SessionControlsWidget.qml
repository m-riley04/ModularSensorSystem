import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import app.styles.MSSDark
import app.actions
import app.components.plugins

ColumnLayout {
  id: root
  signal openCloseClicked
  signal startStopClicked
  signal restartClicked
  signal recordClicked
  signal clipClicked
  signal propertiesClicked

  spacing: 3
  Layout.fillWidth: true
  Layout.fillHeight: true
  Button {
    id: buttonOpenCloseDevices
    text: checked ? qsTr("Close Sources") : qsTr("Open Sources")
    ToolTip.text: qsTr("Opens or closes all of the sources in the session")
    checkable: true
    checked: false
    onClicked: {
      checked ? SourcesActions.openSources.trigger(
                  ) : SourcesActions.closeSources.trigger()
      root.openCloseClicked()
    }
  }
  Button {
    id: buttonStartStopDevices
    text: checked ? qsTr("Stop Sources") : qsTr("Start Sources")
    ToolTip.text: qsTr("Starts or stops all sources in the session")
    checkable: true
    checked: false
    onClicked: {
      checked ? SourcesActions.startSources.trigger(
                  ) : SourcesActions.stopSources.trigger()
      root.startStopClicked()
    }
  }
  Button {
    id: buttonRestartDevices
    text: qsTr("Restart Sources")
    icon.name: "system-reboot"
    ToolTip.text: qsTr("Restarts all sources in the session")
    onClicked: {
      root.restartClicked()
    }
  }
  Button {
    id: buttonRecord
    text: checked ? qsTr("Stop Recording") : qsTr("Start Recording")
    icon.name: "media-record"
    ToolTip.text: qsTr("Starts recording of all sources")
    checkable: true
    checked: false
    onClicked: {
      root.recordClicked()
    }
  }
  Button {
    id: buttonClip
    text: qsTr("Capture Clip")
    ToolTip.text: qsTr("Captures a clip of the current sources")
    onClicked: {
      root.clipClicked()
    }
  }
  Button {
    id: buttonProperties
    text: qsTr("Properties")
    icon.name: "document-properties"
    ToolTip.text: qsTr("Opens the session's properties")
    onClicked: {
      propertiesWindow.show()
      root.propertiesClicked()
    }
  }

  DialogWindow {
    id: propertiesWindow
    title: qsTr("Properties")
  }
}
