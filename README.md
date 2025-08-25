# PhytoNode-Plant-Monitor
A professional embedded systems project featuring an ESP32-based device that monitors environmental conditions (soil moisture, temperature, humidity, light) and transmits data to the cloud for real-time visualization.

## 🚀 Features
- Real-time monitoring of plant health metrics.
- Wi-Fi connectivity using the ESP32.
- Data transmission to the cloud via MQTT.
- Web dashboard (ThingSpeak) for remote monitoring.
- Battery-powered operation for true wireless deployment.

## 📸 Media
**(Add pictures here!)**
- Image 1: The finished device next to a plant.
- Image 2: The internal wiring (make it neat!).
- Image 3: A screenshot of the ThingSpeak dashboard with your live data.

## 🛠️ Hardware & Wiring
- Microcontroller: ESP32 DEVKIT V1
- Sensors:
    - Capacitive Soil Moisture Sensor
    - DHT22 (AM2302) Temperature & Humidity Sensor
    - Photoresistor (LDR) with 10kΩ resistor
- Power: 3.7V LiPo Battery (1000mAh) with TP4056 Charging Module

*Table: ESP32 Pinout*
| Sensor       | ESP32 Pin |
|--------------|-----------|
| Soil Moisture| GPIO 34   |
| DHT22        | GPIO 17   |
| LDR          | GPIO 35   |

## ⚙️ Software
- **Framework:** Arduino (C++) on PlatformIO
- **Key Libraries:** `DHT sensor library`, `PubSubClient` (for MQTT), `ThingSpeak`
- **Protocol:** MQTT
- **Cloud Platform:** ThingSpeak

## 📁 Project Structure
PhytoNode/
├── src/
│ └── main.cpp # Main firmware code
├── include/ # Header files (if any)
├── platformio.ini # PlatformIO configuration
└── README.md

text

## 🔧 Installation & Setup
1.  Clone this repo: `git clone https://github.com/KGobikrishnan/PhytoNode-Plant-Monitor.git`
2.  Open the project in VS Code with PlatformIO.
3.  Update `src/secrets.example.h` with your Wi-Fi and API credentials. Rename it to `secrets.h`.
4.  Upload the code to your ESP32.


## 📝 License
This project is open source and available under the [MIT License](LICENSE).
