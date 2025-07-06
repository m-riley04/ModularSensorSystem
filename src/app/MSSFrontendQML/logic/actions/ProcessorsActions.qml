import QtQuick
import QtQuick.Controls

ActionGroup {
	property alias addProcessor: actionAddProcessor;
    property alias removeProcessor: actionRemoveProcessor;
    property alias configureProcessor: actionConfigureProcessor;
    property alias toggleProcessing: actionToggleProcessing;

    Action { id: actionAddProcessor; text: qsTr("Add Processor") }
    Action { id: actionRemoveProcessor; text: qsTr("Remove Processor") }
    Action { id: actionConfigureProcessor; text: qsTr("Configure Processor") }
    Action { id: actionToggleProcessing; text: qsTr("Toggle Processing"); checkable: true; }
}
