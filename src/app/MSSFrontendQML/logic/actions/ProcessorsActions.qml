import QtQuick
import QtQuick.Controls

ActionGroup {
	property alias addProcessor: actionAddProcessor;
    property alias removeProcessor: actionRemoveProcessor;
    property alias configureProcessor: actionConfigureProcessor;
    property alias toggleProcessing: actionToggleProcessing;

    Action { id: actionAddProcessor; text: qsTr("Add Processor"); icon.name: "list-add"; }
    Action { id: actionRemoveProcessor; text: qsTr("Remove Processor"); icon.name: "list-remove"; }
    Action { id: actionConfigureProcessor; text: qsTr("Configure Processor"); icon.name: "document-properties"; }
    Action { id: actionToggleProcessing; text: qsTr("Toggle Processing"); icon.name: "media-playback-start"; checkable: true; }
}
