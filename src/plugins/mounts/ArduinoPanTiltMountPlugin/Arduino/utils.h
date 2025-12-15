#pragma once

#include "constants.h"
#include <Arduino.h>

inline void sendJoystickMessage(arduino::Stream& stream, int x, int y) {
  stream.print("x=");
  stream.print(x);
  stream.print("&y=");
  stream.println(y);
}

inline void sendRotationMessage(arduino::Stream& stream, int yaw, int pitch) {
  stream.print("yaw=");
  stream.print(yaw);
  stream.print("&pitch=");
  stream.println(pitch);
}

inline void sendErrorMessage(arduino::Stream& stream, arduino::String& str) {
  stream.print("ERROR: ");
  stream.println(str);
}

inline void sendInfoMessage(arduino::Stream& stream, int yaw, int pitch, int minYaw, int maxYaw, int minPitch, int maxPitch) {
  stream.print("minYaw=");
  stream.print(minYaw);
  stream.print("&maxYaw=");
  stream.print(maxYaw);
  stream.print("&minPitch=");
  stream.print(minPitch);
  stream.print("&maxPitch=");
  stream.print(maxPitch);
  stream.print("&yaw=");
  stream.print(yaw);
  stream.print("&pitch=");
  stream.println(pitch);
}