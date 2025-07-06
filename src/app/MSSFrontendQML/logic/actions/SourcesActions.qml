import QtQuick
import QtQuick.Controls

ActionGroup {
    property alias addSource: actionAddSource;
    property alias removeSource: actionRemoveSource;
    property alias configureSource: actionConfigureSource;
    Action { id: actionAddSource; text: qsTr("Add Source") }
    Action { id: actionRemoveSource; text: qsTr("Remove Source") }
    Action { id: actionConfigureSource; text: qsTr("Configure Source") }
}
