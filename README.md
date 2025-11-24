#  ESP32 Sensor Nodes 

This branch contains the code for both **ESP32 sensor nodes** used in the *Cooperative Thermal Alert Network (CTAN)*.  
Thus, each node measures **temperature** and **humidity**  and publishes sensor data securely to **HiveMQ Cloud (MQTT server over TLS)**.

---

##  Overview

Each ESP32 sensor node includes:

- **DHT11 sensor** temperature & humidity sensor  
- **NeoPixel RGB LED** for color-coded status  
- **WiFi connection** to HiveMQ Cloud  
- **Secure MQTT server** using TLS  
- JSON-encoded sensor payloads published at regular intervals  

This configuration runs independently on each node and reports environmental data to MQTT topics.

---

##  Features

###  Temperature & Humidity Monitoring  
Reads environmental data using a DHT11 sensor.

###  Color-Coded LED Status  
The NeoPixel LED indicates thermal conditions:
| Condition | Temperature Range | LED Color |
|----------|-------------------|-----------|
|  Cold | `< 18°C` | Blue |
|  Optimal | `18–30°C` | Green |
|  Hot | `> 30°C` | Red |

###  Secure MQTT Publishing (TLS)  
Data is published to **HiveMQ Cloud** via port **8883** using `WiFiClientSecure` library.

###  JSON Sensor Payload  
Nodes publish compact JSON messages:
```json
{
  "node": "X",
  "name": "nodeX",
  "temperature": 26.7,
  "humidity": 52,
  "status": "Optimal"
}
