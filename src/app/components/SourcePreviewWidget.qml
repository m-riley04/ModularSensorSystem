import QtQuick
import QtQuick.Window

Item {
    id: sourcePreviewWidget
    property alias image: previewImage.source
    property bool hasFrame: previewImage.status === Image.Ready
    signal frameChanged(url imageUrl)

    // The image to display (set by the backend or a controller)
    Image {
        id: previewImage
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit
        source: "" // Set this to the frame URL or data
        onStatusChanged: {
            if (status === Image.Ready) {
                sourcePreviewWidget.frameChanged(source)
            }
        }
    }
}
