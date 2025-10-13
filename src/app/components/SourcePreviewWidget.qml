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
    anchors.fill: root

    Connections {
      target: SourcesActions.startSources
      function onTriggered() {
        camera.start()
        propertiesPopup.show()
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

    ColumnLayout {
      anchors.fill: parent

      VideoOutput {
        id: videoOutput
        Layout.fillWidth: true
        Layout.fillHeight: true
      }

      Camera {
        id: camera
      }

      CaptureSession {
        id: captureSession
        camera: camera
        videoOutput: videoOutput
      }

      RowLayout {
        z: 999
        Button {
          text: qsTr("Start Camera")
          onClicked: SourcesActions.startSources.trigger()
        }

        Button {
          text: qsTr("Stop Camera")
          onClicked: SourcesActions.stopSources.trigger()
        }
      }
    }
  }

  // The image to display (set by the backend or a controller)
  // Image {
  //   id: sourcePreviewWidget
  //   Layout.fillWidth: true
  //   Layout.fillHeight: true

  //   property alias image: sourcePreviewWidget.source
  //   property bool hasFrame: sourcePreviewWidget.status === Image.Ready

  //   signal frameChanged(url imageUrl)

  //   fillMode: Image.PreserveAspectFit
  //   source: "" // Set this to the frame URL or data
  //   onStatusChanged: {
  //     if (status === Image.Ready) {
  //       sourcePreviewWidget.frameChanged(source)
  //     }
  //   }
  // }
}
