#pragma once

#include "constants.h"

inline int calculateYaw(int x, int currentYaw) {
  int newYaw = currentYaw;

  // Change the yaw
  if (x > DEFAULT_X + DEADZONE_RADIUS) {
    newYaw++;
  } else if (x < DEFAULT_X - DEADZONE_RADIUS) {
    newYaw--;
  }

  // Check yaw bounds
  if (newYaw > MAX_YAW_ANGLE) newYaw = MAX_YAW_ANGLE;
  if (newYaw < MIN_YAW_ANGLE) newYaw = MIN_YAW_ANGLE;

  return newYaw;
}

inline int calculatePitch(int y, int currentPitch) {
  int newPitch = currentPitch;

  // Change the pitch
  if (y > DEFAULT_Y + DEADZONE_RADIUS) {
    newPitch++;
  } else if (y < DEFAULT_Y - DEADZONE_RADIUS) {
    newPitch--;
  }

  // Check pitch bounds
  if (newPitch > MAX_PITCH_ANGLE) newPitch = MAX_PITCH_ANGLE;
  if (newPitch < MIN_PITCH_ANGLE) newPitch = MIN_PITCH_ANGLE;

  return newPitch;
}