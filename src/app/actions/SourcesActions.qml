pragma Singleton

import QtQuick
import QtQuick.Controls

ActionGroup {
  property alias addSource: actionAddSource
  property alias removeSource: actionRemoveSource
  property alias configureSource: actionConfigureSource
  property alias openSources: actionOpenSources
  property alias closeSources: actionCloseSources
  property alias startSources: actionStartSources
  property alias stopSources: actionStopSources
  Action {
    id: actionAddSource
    text: qsTr("Add Source")
    icon.name: "list-add"
  }
  Action {
    id: actionRemoveSource
    text: qsTr("Remove Source")
    icon.name: "list-remove"
  }
  Action {
    id: actionConfigureSource
    text: qsTr("Configure Source")
    icon.name: "document-properties"
  }
  Action {
    id: actionOpenSources
    text: qsTr("Open Sources")
  }
  Action {
    id: actionCloseSources
    text: qsTr("Close Sources")
  }
  Action {
    id: actionStartSources
    text: qsTr("Start Sources")
    onTriggered: console.log("Starting sources...")
  }
  Action {
    id: actionStopSources
    text: qsTr("Stop Sources")
    onTriggered: console.log("Stopping sources...")
  }
}
