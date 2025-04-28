# 🪑 Desk Posture Reminder (Arduino + Ultrasonic Sensor)

## Overview
The **Desk Posture Reminder** is an Arduino-based project designed to promote healthy desk posture.  
It uses an **ultrasonic sensor** to measure your sitting distance and intelligently calibrates to your natural posture.  
After calibration, it monitors your distance and gently alerts you if you slouch or lean too close.

## Features
- 📏 **Smart Calibration** — automatically sets a baseline based on the user's correct sitting distance.
- 🧠 **Posture Monitoring** — detects if posture becomes bad (too close) or good (at calibrated distance).
- 🔔 **Visual and Audio Alerts** — LED flashes and/or buzzer sounds if poor posture is detected.
- 🛠️ **Customizable Thresholds** — fine-tune calibration settings easily in the Arduino code.
- 🔋 **Low Power Consumption** — designed for long desk sessions.

## Components Used
- Arduino Uno R3 (or compatible board)
- HC-SR04 Ultrasonic Distance Sensor
- LED (for visual alert)
- Buzzer (for audio alert)
- 220Ω Resistor (for LED)
- Breadboard and Jumper Wires
- USB Cable (for programming and power)

## How It Works
1. When powered on, the system **calibrates** by measuring the user's initial sitting distance.
2. This baseline distance is saved as the reference for **good posture**.
3. If the user moves significantly closer (slouching), the system triggers a **reminder alert**.
4. Returning to the calibrated distance stops the alert, encouraging proper posture.

## Basic Wiring
| Component           | Arduino Pin |
|:--------------------|:------------|
| HC-SR04 VCC          | 5V          |
| HC-SR04 GND          | GND         |
| HC-SR04 Trig         | Pin 9       |
| HC-SR04 Echo         | Pin 10      |
| LED Positive Lead    | Pin 12 (with 220Ω resistor) |
| Buzzer Positive Lead | Pin 8       |
| GND Connections      | GND         |

## Setup Instructions
1. Connect all components according to the wiring table.
2. Upload the Desk Posture Reminder sketch to your Arduino board.
3. Sit at your desk in your **natural good posture** during power-up to allow calibration.
4. After calibration, the device will monitor your sitting distance and alert you if you slouch.

## Future Improvements
- Add a button to **manually recalibrate** at any time.
- Integrate a display to show posture status or timer.
- Bluetooth connectivity for posture tracking apps.
- Battery-powered and portable version.

## License
This project is open-source under the **MIT License**.  
Feel free to use, modify, and enhance it!

---