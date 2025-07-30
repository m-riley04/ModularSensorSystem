import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    id: root

    property alias title: root.title = qsTr("Dialog Window")
    property alias buttons: dialog.standardButtons
    property alias onAccepted: dialog.onAccepted
    property alias onRejected: dialog.onRejected
    default property alias content: contentHost.data

    Dialog {
        id: dialog
        width: root.width
        height: root.height
        standardButtons: Dialog.Ok | Dialog.Cancel

        Item {
            id: contentRoot
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}
