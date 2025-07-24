import QtQuick
import QtQuick.Window

// The image to display (set by the backend or a controller)
Image {
  id: sourcePreviewWidget

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
