#include <USBHost.h>
#include <Servo.h>
#include "config.h"

USBHost usb;
USBHub hub(usb);
HIDJoystick joystick(usb);
Servo steeringServo;
Servo throttleServo;

void setup() {
  Serial.begin(SERIAL_BAUD);
  usb.begin();
  
  steeringServo.attach(STEERING_PIN);
  throttleServo.attach(THROTTLE_PIN);
  
  // Initialize to neutral position
  steeringServo.write(SERVO_NEUTRAL);
  throttleServo.write(SERVO_NEUTRAL);
  
  #ifdef DEBUG
  Serial.println("RC Controller initialized");
  #endif
}

void loop() {
  usb.Task();
  
  if (joystick.available()) {
    processJoystickInput();
  }
  
  // Add other sensor readings or functions here
  
  delay(LOOP_DELAY);
}

void processJoystickInput() {
  int steeringRaw = joystick.getAxis(STEERING_AXIS);
  int throttleRaw = joystick.getAxis(THROTTLE_AXIS);
  
  int steeringMapped = map(steeringRaw, 
                          JOYSTICK_MIN, JOYSTICK_MAX, 
                          STEERING_MIN, STEERING_MAX);
  int throttleMapped = map(throttleRaw, 
                          JOYSTICK_MIN, JOYSTICK_MAX, 
                          THROTTLE_MIN, THROTTLE_MAX);
  
  // Apply smoothing or filtering if needed
  steeringServo.write(steeringMapped);
  throttleServo.write(throttleMapped);
  
  #ifdef DEBUG
  Serial.print("Steering: ");
  Serial.print(steeringMapped);
  Serial.print(" Throttle: ");
  Serial.println(throttleMapped);
  #endif
}