import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts
import QtMultimedia
import app.components.plugins

Rectangle {
  id: root

  Item {
    id: testCamera
    Layout.fillWidth: true
    Layout.fillHeight: true

    Connections {
      target: SourcesActions.startSources
      function onTriggered() {
        camera.start()
        propertiesPopup.open();
      }
    }

    Connections {
      target: SourcesActions.stopSources
      function onTriggered() {
        camera.stop()
      }
    }

    DialogWindow {
      id: propertiesPopup
      width: 300
      height: 300
    }

    RowLayout {
      z: 999
      anchors.fill: parent
      Button {
        text: qsTr("Start Camera")
        onClicked: SourcesActions.startSources.trigger()
      }

      Button {
        text: qsTr("Stop Camera")
        onClicked: SourcesActions.stopSources.trigger()
      }
    }

    Camera {
      id: camera
    }

    VideoOutput {
      id: videoOutput
      anchors.fill: parent
    }

    CaptureSession {
      id: captureSession
      camera: camera
      videoOutput: videoOutput
    }
  }

  // The image to display (set by the backend or a controller)
  Image {
    id: sourcePreviewWidget
    Layout.fillWidth: true
    Layout.fillHeight: true

    property alias image: sourcePreviewWidget.source
    property bool hasFrame: sourcePreviewWidget.status === Image.Ready

    signal frameChanged(url imageUrl)

    fillMode: Image.PreserveAspectFit
    source: "" // Set this to the frame URL or data
    onStatusChanged: {
      if (status === Image.Ready) {
        sourcePreviewWidget.frameChanged(source)
      }
    }
  }
}
