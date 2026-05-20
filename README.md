# Popiot — Industry 4.0 IoT Demonstrator

A hands-on IoT demonstration platform built around a **Connected Popcorn Machine** that simulates real-world Industry 4.0 concepts including sensor data acquisition, MQTT messaging, cloud dashboards, and automated control.

---

## Overview

Popiot demonstrates a complete IoT stack from embedded sensor nodes to a web-based monitoring dashboard:

- **ESP8266/ESP32 sensor nodes** collect temperature, humidity, and distance data
- **MQTT broker (Mosquitto)** handles publish/subscribe messaging
- **Node-RED** processes flows and automates relay control
- **InfluxDB** stores time-series sensor data
- **Grafana** visualizes live and historical data on dashboards
- **Raspberry Pi** acts as the central gateway and server

---

## Architecture

```
[ESP8266 Sensors] --MQTT--> [Raspberry Pi]
                                    |
                    +---------------+---------------+
                    |               |               |
              [Mosquitto]     [Node-RED]       [InfluxDB]
              (Broker)        (Automation)    (Storage)
                                    |
                              [Grafana]
                              (Dashboard)
```

---

## Tech Stack

| Layer | Technology |
|---|---|
| Microcontroller | ESP8266 (Arduino framework) |
| Messaging | MQTT / Mosquitto Broker |
| Flow Automation | Node-RED |
| Time-Series DB | InfluxDB |
| Visualization | Grafana |
| Gateway | Raspberry Pi |

---

## Repository Structure

```
popiot/
├── relayesp-esp2/          # ESP8266 relay control firmware
├── temp_hum_distance-esp1/ # ESP8266 sensor firmware (temp/humidity/distance)
├── node_red_flow.json      # Node-RED flow export
├── grafana.json            # Grafana dashboard export
```

---

## Getting Started

### 1. Setup the Server Stack (Raspberry Pi)

Install Mosquitto, Node-RED, InfluxDB, and Grafana on your Raspberry Pi. Configure Node-RED to subscribe to MQTT topics and write data to InfluxDB.

### 2. Flash ESP8266 Firmware

Flash the sensor firmware from `temp_hum_distance-esp1/` and the relay firmware from `relayesp-esp2/` using Arduino IDE. Update the WiFi and MQTT broker credentials in the sketch.

### 3. Import Dashboards

Import `node_red_flow.json` into Node-RED and `grafana.json` into Grafana to get the pre-built automation flows and dashboard.

---

## License

MIT
