pragma Singleton

import QtQuick
import QtQuick.Controls

ActionGroup {
    property alias clipping: actionClipping
    property alias sessionOptions: actionSessionOptions
    Action {
        id: actionClipping
        text: qsTr("Clipping")
        checkable: true
    }
    Action {
        id: actionSessionOptions
        text: qsTr("Options")
    }
}
