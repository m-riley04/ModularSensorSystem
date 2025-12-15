#include <Servo.h>
#include <string.h>
#include "constants.h"
#include "pantilt.h"
#include "messages.h"
#include "utils.h"

int x = DEFAULT_X, y = DEFAULT_Y, prevX = DEFAULT_X, prevY = DEFAULT_Y;
int yaw = DEFAULT_YAW, pitch = DEFAULT_PITCH, prevYaw = DEFAULT_YAW, prevPitch = DEFAULT_PITCH;
bool joystickChanged = false, anglesChanged = false;
char msgBuf[MESSAGE_BUF_SIZE];
Servo servoYaw;
Servo servoPitch;

bool debug = true;

void handlePositionMessage(String msg) {
  PositionMessage posMsg = parsePositionMessageString(msg);
  if (posMsg.ok) {
    yaw = posMsg.yaw;
    pitch = posMsg.pitch;
  } else {
    Serial.print("ERROR: ");
    Serial.println(posMsg.err);
  }
}

void setup() {
  Serial.begin(BAUD_RATE);

  // Attach the servo pins
  servoYaw.attach(YAW_PIN);
  servoPitch.attach(PITCH_PIN);
}

void loop() {
  // Save previous values
  prevX = x;
  prevY = y;
  prevYaw = yaw;
  prevPitch = pitch;

  // Check for incoming messages
  if (Serial.available() > 0) {
    size_t len = Serial.readBytesUntil('\n', msgBuf, MESSAGE_BUF_SIZE - 1);
    msgBuf[len] = '\0';  // null-terminate

    String msg(msgBuf);  // or parse as C-string directly
    msg.trim();

    MessageType type = checkMessageType(msg);

    switch (type) {
      case MessageType::INFO:
        sendInfoMessage(Serial, yaw, pitch, MIN_YAW_ANGLE, MAX_YAW_ANGLE, MIN_PITCH_ANGLE, MAX_PITCH_ANGLE);
        break;
      case MessageType::POS:
        handlePositionMessage(msg);
        break;
      case MessageType::ERROR:
      default:
        String errMsg = "Unknown message type";
        sendErrorMessage(Serial, errMsg);
    }
    
  } else {
    // Read the values from the joystick
    x = analogRead(VRX_PIN);
    y = analogRead(VRY_PIN);

    // Detect if joystick moved
    joystickChanged = (prevX != x) || (prevY != y);

    // Calculate the yaw and pitch
    yaw = calculateYaw(x, yaw);
    pitch = calculatePitch(y, pitch);
  }
  
  // Detect if angles changed
  anglesChanged = (prevYaw != yaw) || (prevPitch != pitch);

  if (anglesChanged) {
    if (joystickChanged) {
      // Notify listeners of manual change
      sendRotationMessage(Serial, yaw, pitch);
    }

    // Write rotation update to the servos
    servoYaw.write(yaw);
    servoPitch.write(pitch);
  }
}
