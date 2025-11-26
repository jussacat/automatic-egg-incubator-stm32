# Automatic Chicken Egg Incubation Support System using STM32F103RCT6

## Project Overview
This project presents an automatic support system for chicken egg incubation, designed to maintain stable and optimal environmental conditions. The core of the system is the STM32F103RCT6 ARM Cortex-M3 Mini microcontroller, which manages critical parameters like temperature, humidity, and incubation time. The goal is to address the instability of manual incubation methods and improve hatching efficiency for small-scale operations.

> **Note:** This is a student project and may not be fully optimized for commercial use.

---

## Hardware and Key Specifications

* **Microcontroller:** STM32F103RCT6 Mini Kit
* **Sensor:** DHT22 Temperature & Humidity Sensor
* **Heating Element:** Incandescent Lamp 220V – 60W
* **Air Circulation:** AC Axial Fan
* **Input/Output:** LCD 16x2 (I2C) & Keypad 4x4
* **Alerts:** Buzzer Module

### Ideal Parameters
* **Temperature:** $37.5^{\circ}\mathrm{C}$ to $37.8^{\circ}\mathrm{C}$
* **Incubation Time:** 21 days
* **Humidity:** $55\%$ to $65\%$

---

## Software and Operation
The application is developed using **STM32CubeIDE**. The logic is managed by the main program loop and Timer interrupts.

* **Sensing:** The DHT22 reads environment data (Temperature and Humidity).
* **Control:** The system monitors the temperature against user-set thresholds (`temp_high` and `temp_low`).
* **Alerting:** If the temperature deviates outside the safe range ($\ge$ `temp_high` or $\le$ `temp_low`), the buzzer is activated using **PWM (Timer 3)**.
* **Time Tracking:** Timer2 is configured to increment the `incubationDay` variable every 86400 seconds (1 day).

### Configuration Menu
The user can modify the control parameters via the Keypad 4x4:

* Press **`#`** to enter the configuration menu.
* Use **`4` / `6`** to navigate menu items (High Temp, Low Temp, Incubation Day).
* Use **`2`** (Increase) and **`8`** (Decrease) to adjust values.
* Press **`*`** to save settings and exit.

---

## Project Visualization

<img width="780" height="553" alt="image" src="https://github.com/user-attachments/assets/a77c73ee-0082-43ad-9cdc-097d24eaa7c9" />
<img width="456" height="256" alt="image" src="https://github.com/user-attachments/assets/befa97a5-f115-4711-8bd6-cc950ac17ef9" />
<img width="442" height="258" alt="image" src="https://github.com/user-attachments/assets/04039e9e-4db2-42da-9032-e09f698f414b" />
<img width="449" height="253" alt="image" src="https://github.com/user-attachments/assets/285786d9-7452-4881-8c13-882dde473649" />

---

## Future Scope
Potential development directions for this project include:

* **IoT Integration:** Implementing Wi-Fi connectivity for remote monitoring and mobile alerts.
* **Advanced Control Algorithms:** Using a PID algorithm to optimize and stabilize temperature and humidity control.
* **Mechanical Optimization:** Refining the speed and mechanism of the automatic egg-turning system.

---

## Developed By
Group 8, 23DTV\_CLC2 - Faculty of Electronics and Telecommunications, University of Science, VNU-HCM

## References
1. STMicroelectronics, STM32F103RC Datasheet, STMicroelectronics (2025).
2. SparkFun Electronics, DHT22 Sensor Datasheet, SparkFun (2025).
3. Alpha & Omega Semiconductor, AOB4184 Datasheet, Alpha & Omega Semiconductor (2025).
4. RS Components, AC Axial Fan Datasheet, RS Components (2025).
5. Parallax Inc, 4x4 Matrix Membrane Keypad, Parallax Inc (2011).
6. STMicroelectronics, ST-LINK/V2 in-circuit debugger/programmer for STM8 and STM32, STMicroelectronics (2020).
