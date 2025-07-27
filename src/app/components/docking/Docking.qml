pragma Singleton

import QtQml

QtObject {

  /**
    * Represents the zone of where a dock can be in the default root.
    */
  enum DockZone {
    NONE,
    LEFT,
    RIGHT,
    TOP,
    BOTTOM,
    CENTER
  }
}
