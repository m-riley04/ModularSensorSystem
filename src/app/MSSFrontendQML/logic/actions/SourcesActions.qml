import QtQuick
import QtQuick.Controls

ActionGroup {
    property alias addSource: actionAddSource;
    property alias removeSource: actionRemoveSource;
    property alias configureSource: actionConfigureSource;
    Action { id: actionAddSource; text: qsTr("Add Source"); icon.name: "list-add"; }
    Action { id: actionRemoveSource; text: qsTr("Remove Source"); icon.name: "list-remove"; }
    Action { id: actionConfigureSource; text: qsTr("Configure Source"); icon.name: "document-properties"; }
}
