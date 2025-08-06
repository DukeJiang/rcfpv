// Hardware Configuration
#define STEERING_PIN 9
#define THROTTLE_PIN 10
#define SERVO_NEUTRAL 90

// Joystick Configuration
#define STEERING_AXIS 0  // Typically X-axis
#define THROTTLE_AXIS 1  // Typically Y-axis
#define JOYSTICK_MIN -32768
#define JOYSTICK_MAX 32767

// Servo Limits (adjust for your RC car)
#define STEERING_MIN 45   // Full left
#define STEERING_MAX 135  // Full right
#define THROTTLE_MIN 60   // Full reverse
#define THROTTLE_MAX 120  // Full forward

// System Settings
#define SERIAL_BAUD 115200
#define LOOP_DELAY 20  // ms

// Debug Options
// #define DEBUG