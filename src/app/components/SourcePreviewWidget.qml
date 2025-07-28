import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts
import QtMultimedia

// The image to display (set by the backend or a controller)
// Image {
//   id: sourcePreviewWidget

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
Rectangle {
  id: root

  Connections {
    target: SourcesActions.startSources
    function onTriggered() {
      camera.start()
    }
  }

  Connections {
    target: SourcesActions.stopSources
    function onTriggered() {
      camera.stop()
    }
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
