import QtQuick
import QtQuick.Layouts

Rectangle {
  id: zone

  property int targetZone: Docking.DockZone.NONE
  property bool isHighlighted: false

  property color bgColor: Qt.rgba(0.0, 0.6, 1.0)
  property double defaultAlpha: 0.15
  property double highlightAlpha: 0.35
  property int borderRadius: 8

  property color borderColor: Qt.rgba(0, 0.6, 1)
  property double borderAlpha: 0.5

  color: Qt.rgba(bgColor.r, bgColor.g, bgColor.b,
                 isHighlighted ? highlightAlpha : defaultAlpha)
  radius: borderRadius
  border.color: Qt.rgba(borderColor.r, borderColor.g, borderColor.b,
                        borderAlpha)
}
