# 🌱 Soil Moisture Sensor (ESP32) + I2C LCD — Wokwi Simulation

A simple ESP32 project that reads soil moisture from **GPIO 35** and shows the status on a **16×2 I2C LCD**.

---

## 📸 Simulation Preview
![Wokwi Simulation](assets/wokwi-sim.png.png)

---

## 🔌 Wiring (ESP32)
| Component | Pin |
|---|---|
| LCD SDA | GPIO 21 |
| LCD SCL | GPIO 22 |
| Sensor AO | GPIO 35 |

---

## 🧪 How it Works
- The sensor value is mapped to **0–100%** using calibration values (**dryValue / wetValue**).
- Moisture is classified into: **DRY / NORMAL / WET**
- A stability check prevents LCD flickering when readings fluctuate.

---

## 📁 Project Structure
- `simulation/diagram.json` → Wokwi wiring
- `simulation/soil_sensor.ino` → Simulation code
- `simulation/libraries.txt` → Libraries used
- `assets/wokwi-sim.png` → Screenshot preview
