# ak-robotech25

Source code for Robotech 2025 project.

Team members: Adityan Swaminathan, Akash Sarode, Raj Kothari, Amogh Kondaji

## Keypad Door Lock
With Buzzcard activated locks becoming more widespread, students commonly get locked out of their room due to forgetting their Buzzcard. To solve this problem, we created an additional door unlocking mechanism that uses a keypad for authentication. There are two ESP32 microcontrollers, one on each side of the door. 

The ESP32 outside of the door takes input from a capacitive touch keypad. The user enters a 6-digit code. T264aered is displayed on an LCD screen. Upon entering a correct code, the outside ESP32 sends data to the ESP on the other side through the ESP-NOW wireless communication protocol.

On the other side of the door, there is another ESP32. This ESP controls a stepper motor. Using a 3D printed gear connected to the motor and a bar with teeth, we implemented a linear actuator. The actuator pushes the door handle down to unlock the door. 

We also utilize Blynk, an IoT platform, to provide an interface for the user to interact with the door lock. Blynk allows a phone to send data to an ESP32 and vice versa. Currently, the user can change the door lock’s password. Additionally, they get a push notification when the door unlocks. 
