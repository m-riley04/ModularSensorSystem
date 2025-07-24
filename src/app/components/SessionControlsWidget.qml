import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import app.styles.MSSDark

ColumnLayout {
  id: sessionControlsWidget
  signal openCloseClicked
  signal startStopClicked
  signal restartClicked
  signal recordClicked
  signal clipClicked
  signal propertiesClicked

  spacing: 3
  Button {
    id: buttonOpenCloseDevices
    text: qsTr("Open Devices")
    ToolTip.text: qsTr("Opens or closes all of the devices in the session")
    onClicked: sessionControlsWidget.openCloseClicked()
  }
  Button {
    id: buttonStartStopDevices
    text: qsTr("Start Devices")
    ToolTip.text: qsTr("Starts or stops all sources in the session")
    onClicked: sessionControlsWidget.startStopClicked()
  }
  Button {
    id: buttonRestartDevices
    text: qsTr("Restart Devices")
    icon.name: "system-reboot"
    ToolTip.text: qsTr("Restarts all sources in the session")
    onClicked: sessionControlsWidget.restartClicked()
  }
  Button {
    id: buttonRecord
    text: qsTr("Start Recording")
    icon.name: "media-record"
    ToolTip.text: qsTr("Starts recording of all sources")
    onClicked: sessionControlsWidget.recordClicked()
  }
  Button {
    id: buttonClip
    text: qsTr("Capture Clip")
    ToolTip.text: qsTr("Captures a clip of the current sources")
    onClicked: sessionControlsWidget.clipClicked()
  }
  Button {
    id: buttonProperties
    text: qsTr("Properties")
    icon.name: "document-properties"
    ToolTip.text: qsTr("Opens the session's properties")
    onClicked: sessionControlsWidget.propertiesClicked()
  }
}
