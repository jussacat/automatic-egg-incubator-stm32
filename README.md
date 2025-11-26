# Automatic Chicken Egg Incubation Support System using STM32F103RCT6

## Project Overview
[cite_start]This project presents an automatic support system for chicken egg incubation, designed to maintain stable and optimal environmental conditions [cite: 6, 88-92]. [cite_start]The core of the system is the **STM32F103RCT6 ARM Cortex-M3 Mini** microcontroller [cite: 6, 101, 163][cite_start], which manages critical parameters like temperature, humidity, and incubation time[cite: 142]. [cite_start]The goal is to address the instability of manual incubation methods and improve hatching efficiency for small-scale operations[cite: 95, 98].

> **Note:** This is a student project and may not be fully optimized for commercial use.

---

## Hardware and Key Specifications

* [cite_start]**Microcontroller:** STM32F103RCT6 Mini Kit [cite: 122, 163]
* [cite_start]**Sensor:** DHT22 Temperature & Humidity Sensor [cite: 104, 125, 181]
* [cite_start]**Heating Element:** Incandescent Lamp 220V – 60W [cite: 106, 129, 203]
* [cite_start]**Input/Output:** LCD 16x2 (I2C) & Keypad 4x4 [cite: 103, 108, 124, 131, 199]
* [cite_start]**Air Circulation:** AC Axial Fan [cite: 105, 126]
* [cite_start]**Alerts:** Buzzer Module [cite: 107, 130]

### Ideal Parameters
* [cite_start]**Temperature:** $37.5^{\circ}\mathrm{C}$ to $37.8^{\circ}\mathrm{C}$ [cite: 146]
* [cite_start]**Incubation Time:** 21 days [cite: 153]
* [cite_start]**Humidity:** $55\%$ to $65\%$ [cite: 149]

---

## Software and Operation
[cite_start]The application is developed using **STM32CubeIDE**[cite: 140]. The logic is managed by the main program loop and Timer interrupts.

* [cite_start]**Sensing:** The DHT22 reads environment data (Temperature and Humidity)[cite: 237, 238].
* [cite_start]**Control:** The system monitors the temperature against user-set thresholds (`temp_high` and `temp_low`)[cite: 244].
* [cite_start]**Alerting:** If the temperature deviates outside the safe range ($\ge$ `temp_high` or $\le$ `temp_low`), the buzzer is activated using **PWM (Timer 3)**[cite: 244, 215].
* [cite_start]**Time Tracking:** Timer2 is configured to increment the `incubationDay` variable every 86400 seconds (1 day)[cite: 253, 254].

### Configuration Menu
[cite_start]The user can modify the control parameters via the Keypad 4x4[cite: 247]:

* [cite_start]Press **`#`** to enter the configuration menu[cite: 247].
* [cite_start]Use **`4` / `6`** to navigate menu items (High Temp, Low Temp, Incubation Day)[cite: 249].
* [cite_start]Use **`2`** (Increase) and **`8`** (Decrease) to adjust values[cite: 250].
* [cite_start]Press **`*`** to save settings and exit[cite: 251].

---

## Project Visualization

<img width="780" height="553" alt="image" src="https://github.com/user-attachments/assets/a77c73ee-0082-43ad-9cdc-097d24eaa7c9" />
<img width="456" height="256" alt="image" src="https://github.com/user-attachments/assets/befa97a5-f115-4711-8bd6-cc950ac17ef9" />
<img width="442" height="258" alt="image" src="https://github.com/user-attachments/assets/04039e9e-4db2-42da-9032-e09f698f414b" />
<img width="449" height="253" alt="image" src="https://github.com/user-attachments/assets/285786d9-7452-4881-8c13-882dde473649" />

---

## Future Scope
Potential development directions for this project include:

* [cite_start]**IoT Integration:** Implementing Wi-Fi connectivity for remote monitoring and mobile alerts[cite: 264].
* [cite_start]**Advanced Control Algorithms:** Using a PID algorithm to optimize and stabilize temperature and humidity control[cite: 265].
* [cite_start]**Mechanical Optimization:** Refining the speed and mechanism of the automatic egg-turning system[cite: 266].

---

## Developed By
[cite_start]Group 8, 23DTV\_CLC2 - Faculty of Electronics and Telecommunications, University of Science, VNU-HCM [cite: 7, 8]

## References
1. [cite_start]STMicroelectronics, STM32F103RC Datasheet, STMicroelectronics (2025)[cite: 297].
2. [cite_start]SparkFun Electronics, DHT22 Sensor Datasheet, SparkFun (2025)[cite: 298].
3. [cite_start]Alpha & Omega Semiconductor, AOB4184 Datasheet, Alpha & Omega Semiconductor (2025)[cite: 299].
4. [cite_start]RS Components, AC Axial Fan Datasheet, RS Components (2025)[cite: 300].
5. [cite_start]Parallax Inc, 4x4 Matrix Membrane Keypad, Parallax Inc (2011)[cite: 301].
6. [cite_start]STMicroelectronics, ST-LINK/V2 in-circuit debugger/programmer for STM8 and STM32, STMicroelectronics (2020)[cite: 302].
