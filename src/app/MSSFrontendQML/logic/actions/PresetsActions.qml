import QtQuick
import QtQuick.Controls

ActionGroup {
	property alias savePreset: actionSavePreset;
	property alias loadPreset: actionLoadPreset;
	property alias refreshPresets: actionRefreshPresets;
	property alias deletePreset: actionDeletePreset;

	Action { id: actionSavePreset; text: qsTr("Save Preset"); }
	Action { id: actionLoadPreset; text: qsTr("Load Preset"); }
	Action { id: actionRefreshPresets; text: qsTr("Refresh Presets"); }
	Action { id: actionDeletePreset; text: qsTr("Delete Preset"); }
}

