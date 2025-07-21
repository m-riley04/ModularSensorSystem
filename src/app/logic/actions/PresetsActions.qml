pragma Singleton

import QtQuick
import QtQuick.Controls

ActionGroup {
	property alias savePreset: actionSavePreset
	property alias loadPreset: actionLoadPreset
	property alias refreshPresets: actionRefreshPresets
	property alias deletePreset: actionDeletePreset

	Action {
		id: actionSavePreset
		text: qsTr("Save Preset")
		icon.name: "document-save"
	}
	Action {
		id: actionLoadPreset
		text: qsTr("Load Preset")
		icon.name: "document-open"
	}
	Action {
		id: actionRefreshPresets
		text: qsTr("Refresh Presets")
		icon.name: "view-refresh"
	}
	Action {
		id: actionDeletePreset
		text: qsTr("Delete Preset")
		icon.name: "edit-delete"
	}
}
