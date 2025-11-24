#  ESP32 Gateway – Cooperative Thermal Alert Network

This branch contains the code for the **ESP32 Gateway** used in the *Cooperative Thermal Alert Network (CTAN)* project.  
The gateway subscribes to all ESP32 sensor node data, processes incoming MQTT messages, relays commands, and provides LED feedback.

---

##  Overview

The ESP32 Gateway acts as the **central communication hub** between IoT sensor nodes and the cloud (HiveMQ).  
It performs three main tasks:

1. **Subscribes** to all sensor node MQTT topics (`greenhouse/+/data`)  
2. **Receives control commands** from the dashboard (`greenhouse/gateway/cmd`)  
3. **Publishes status messages** (`greenhouse/gateway/status`)  

The gateway also includes a **NeoPixel RGB LED** that can be controlled remotely to reflect system state.

---

## Features

### ✔ MQTT Wildcard Subscription  
The gateway subscribes to every node publishing to `greenhouse/+/data` : including `greenhouse/node1/data` and `greenhouse/node2/data`
