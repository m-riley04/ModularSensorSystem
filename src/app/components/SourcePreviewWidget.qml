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
  anchors.fill: parent

  RowLayout {
    z: 999
    anchors.fill: parent
    Button {
      text: qsTr("Start Camera")
      onClicked: camera.start()
    }

    Button {
      text: qsTr("Stop Camera")
      onClicked: camera.stop()
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
