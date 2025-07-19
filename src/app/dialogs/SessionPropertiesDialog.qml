import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Dialog {
    id: sessionPropertiesDialog
    title: qsTr("Session Properties")
    standardButtons: Dialog.Save | Dialog.RestoreDefaults | Dialog.Cancel
    modal: true
    width: 600
    height: 400

    property alias tabIndex: tabWidget.currentIndex
    signal saveClicked()
    signal restoreDefaultsClicked()
    signal cancelClicked()
    signal clippingEnabledChanged(bool enabled)
    signal autoClipChanged(bool enabled)
    signal clearBufferClicked()
    signal pixelFormatChanged(int index)
    signal encoderCodecChanged(int index)
    signal objectDetectionChanged(bool enabled)

    ColumnLayout {
        anchors.fill: parent
        TabView {
            id: tabWidget
            Layout.fillWidth: true
            Layout.fillHeight: true
            Tab {
                title: qsTr("General")
                // Add general session properties here
            }
            Tab {
                title: qsTr("Clipping")
                ColumnLayout {
                    CheckBox {
                        id: clippingEnabled
                        text: qsTr("Enabled")
                        onCheckedChanged: sessionPropertiesDialog.clippingEnabledChanged(checked)
                    }
                    CheckBox {
                        id: autoClip
                        text: qsTr("Auto-Clip")
                        enabled: false
                        onCheckedChanged: sessionPropertiesDialog.autoClipChanged(checked)
                    }
                    GroupBox {
                        title: qsTr("Buffer Status")
                        GridLayout {
                            columns: 2
                            Label { text: qsTr("Frames Queued:") }
                            Label { id: framesQueued; text: "..." }
                            Label { text: qsTr("Max Frames Queued:") }
                            Label { id: maxFramesQueued; text: "..." }
                        }
                    }
                    GroupBox {
                        title: qsTr("Parameters")
                        enabled: false
                        GridLayout {
                            columns: 2
                            Label { text: qsTr("Width") }
                            SpinBox { id: widthSpin }
                            Label { text: qsTr("Height") }
                            SpinBox { id: heightSpin }
                            Label { text: qsTr("FPS") }
                            SpinBox { id: fpsSpin }
                            Label { text: qsTr("Bitrate") }
                            SpinBox { id: bitrateSpin }
                            Label { text: qsTr("Pixel Format") }
                            ComboBox { id: pixelFormatCombo; onCurrentIndexChanged: sessionPropertiesDialog.pixelFormatChanged(currentIndex) }
                            Label { text: qsTr("Encoder Codec") }
                            ComboBox { id: encoderCodecCombo; onCurrentIndexChanged: sessionPropertiesDialog.encoderCodecChanged(currentIndex) }
                        }
                    }
                    Button {
                        text: qsTr("Clear Buffer")
                        onClicked: sessionPropertiesDialog.clearBufferClicked()
                    }
                }
            }
            Tab {
                title: qsTr("Recording")
                // Add recording properties here
            }
            Tab {
                title: qsTr("Processing")
                CheckBox {
                    id: objectDetection
                    text: qsTr("Object Detection")
                    onCheckedChanged: sessionPropertiesDialog.objectDetectionChanged(checked)
                }
            }
        }
    }
    onAccepted: sessionPropertiesDialog.saveClicked()
    onRejected: sessionPropertiesDialog.cancelClicked()
    onReset: sessionPropertiesDialog.restoreDefaultsClicked()
}
