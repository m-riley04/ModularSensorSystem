#pragma once
#include <Arduino.h>
#include <string.h>
#include "constants.h"

struct PositionMessage {
  int yaw;
  int pitch;
  bool ok;
  arduino::String err;
};

enum class MessageType {
  ERROR,
  POS,
  INFO
};

/**
 * Takes a trimmed message string and returns the message type.
 */
inline MessageType checkMessageType(const arduino::String& msg) {
  int commaIdx = msg.indexOf(',');

  if (msg.equalsIgnoreCase("info")) {
    return MessageType::INFO;
  } else if (commaIdx > 0 && commaIdx < (msg.length() - 1)) {
    return MessageType::POS;
  }

  return MessageType::ERROR;
}

/**
 * Messages should be in the format:
 * "yaw,pitch\n"
 * 
 * yaw is "pan", pitch is "tilt"
 */
inline PositionMessage parsePositionMessageString(const arduino::String& msgStr) {
  PositionMessage msg = { 0, 0, false, "Message is empty." };
  if (msgStr.isEmpty()) return msg;

  // Split string by comma
  int idxComma = msgStr.indexOf(',');

  // Handle comma-less messages
  if (idxComma == -1) {
    msg.err = "No comma found in message.";
    return msg;
  }

  arduino::String yawMsg = msgStr.substring(0, idxComma);
  arduino::String pitchMsg = msgStr.substring(idxComma+1);
  yawMsg.trim();
  pitchMsg.trim();

  // Check substrings
  if (yawMsg.isEmpty()) {
    msg.err = "Yaw is empty.";
    return msg;
  }
  if (pitchMsg.isEmpty()) {
    msg.err = "Pitch is empty.";
    return msg;
  }

  // Check bounds
  int readYaw = yawMsg.toInt();
  int readPitch = pitchMsg.toInt();
  if (readYaw < MIN_YAW_ANGLE || readYaw > MAX_YAW_ANGLE) {
    msg.err = "Yaw is under minimum or over maximum.";
    return msg;
  }
  if (readPitch < MIN_PITCH_ANGLE || readPitch > MAX_PITCH_ANGLE) {
    msg.err = "Pitch is under minimum or over maximum.";
    return msg;
  }

  // Write to object
  msg.yaw = readYaw;
  msg.pitch = readPitch;
  msg.ok = true;

  return msg;
}
