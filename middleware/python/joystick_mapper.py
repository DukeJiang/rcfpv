import pygame
import serial
import time
import json
from dataclasses import dataclass

@dataclass
class ControllerConfig:
    port: str = 'COM3'
    baudrate: int = 115200
    steering_axis: int = 0
    throttle_axis: int = 1
    deadzone: float = 0.1
    steering_scale: float = 1.0
    throttle_scale: float = 1.0

class JoystickMapper:
    def __init__(self, config):
        self.config = config
        self.ser = None
        self.running = False
        
        pygame.init()
        pygame.joystick.init()
        
        if pygame.joystick.get_count() == 0:
            raise RuntimeError("No joystick detected")
            
        self.joystick = pygame.joystick.Joystick(0)
        self.joystick.init()
        
    def connect_serial(self):
        try:
            self.ser = serial.Serial(
                self.config.port,
                self.config.baudrate,
                timeout=1
            )
            time.sleep(2)  # Wait for connection
            return True
        except serial.SerialException as e:
            print(f"Serial connection error: {e}")
            return False
    
    def map_value(self, value, deadzone=0.1):
        if abs(value) < deadzone:
            return 0.0
        return value
    
    def run(self):
        if not self.connect_serial():
            return
            
        self.running = True
        print("Controller mapper running. Press Ctrl+C to exit.")
        
        try:
            while self.running:
                pygame.event.pump()
                
                steering = self.joystick.get_axis(self.config.steering_axis)
                throttle = -self.joystick.get_axis(self.config.throttle_axis)
                
                steering = self.map_value(steering, self.config.deadzone) * self.config.steering_scale
                throttle = self.map_value(throttle, self.config.deadzone) * self.config.throttle_scale
                
                # Convert to PWM values (0-180)
                steering_pwm = int(90 + (steering * 45))
                throttle_pwm = int(90 + (throttle * 45))
                
                # Send to Arduino
                cmd = f"{steering_pwm},{throttle_pwm}\n"
                self.ser.write(cmd.encode())
                
                time.sleep(0.02)
                
        except KeyboardInterrupt:
            self.running = False
            self.ser.close()
            print("\nExiting gracefully")

if __name__ == "__main__":
    config = ControllerConfig()
    mapper = JoystickMapper(config)
    mapper.run()