# Industrial IoT Gateway (C++23 + Yocto)

## Overview
This project implements an **Industrial IoT Gateway** in **modern C++23**, designed to run on an embedded Linux system built with **Yocto**.  

The gateway collects data from sensors (via **UART, I²C, PWM**), processes it locally, and publishes telemetry to the cloud using **TCP/IP and MQTT**.

## Custom Linux Yocto distro
    https://github.com/ShayanEram/MPU_Gateway-Yocto

---

## Features
- Multithreaded sensor acquisition and processing  
- Asynchronous networking (TCP/IP, MQTT)  
- Modular OOP architecture with hardware abstraction  
- RAII‑based resource management and smart pointers  
- Templates for thread‑safe queues and ring buffers  
- File I/O for telemetry logging and configuration  
- OTA update checker with async execution  
- Yocto recipe + systemd integration  

---

## 🧩 Project Structure (with detailed responsibilities)

```
gateway-project/
├── CMakeLists.txt              # Build configuration
├── include/                    # Public headers
│   ├── GatewayController.hpp   # Orchestrates all managers
│   ├── SensorManager.hpp       # Manages I2C/UART/PWM sensors
│   ├── DataProcessor.hpp       # Processes raw sensor data
│   ├── CommManager.hpp         # Handles MQTT/TCP/IP comms
│   ├── Logger.hpp              # File logging & telemetry
│   ├── OTAUpdater.hpp          # OTA update checks
│   ├── hw/                     # Hardware abstraction layer
│   │   ├── UARTPort.hpp        # RAII wrapper for UART
│   │   ├── I2CDevice.hpp       # RAII wrapper for I2C
│   │   ├── PWMController.hpp   # PWM control
│   │   └── SocketClient.hpp    # TCP/IP client
│   └── utils/                  # Utilities & templates
│       ├── MessageQueue.hpp    # Thread-safe queue (templates)
│       ├── RingBuffer.hpp      # Circular buffer (templates)
│       ├── ThreadSafeQueue.hpp # Producer-consumer queue
│       └── RAIIHandle.hpp      # Generic RAII resource wrapper
├── src/                        # Implementations
│   ├── main.cpp                # Entry point
│   ├── GatewayController.cpp   # Lifecycle management
│   ├── SensorManager.cpp       # Sensor polling threads
│   ├── DataProcessor.cpp       # Filtering, AI inference
│   ├── CommManager.cpp         # MQTT/TCP client logic
│   ├── Logger.cpp              # File I/O logging
│   ├── OTAUpdater.cpp          # Async OTA logic
│   └── hw/                     # HAL implementations
│       ├── UARTPort.cpp
│       ├── I2CDevice.cpp
│       ├── PWMController.cpp
│       └── SocketClient.cpp
├── tests/                      # Unit & integration tests
│   ├── test_SensorManager.cpp
│   ├── test_CommManager.cpp
│   ├── test_DataProcessor.cpp
│   └── mocks/                  # Mock hardware
│       ├── MockUART.hpp
│       ├── MockI2C.hpp
│       └── MockSocket.hpp
├── configs/                    # Config files
│   └── gateway_config.json     # Defines sensors, comms, logging
├── scripts/                    # Helper scripts
│   ├── run_gateway.sh
│   └── ota_update.sh
├── docs/                       # Documentation
│   ├── architecture.md         # Class diagram & thread flow
│   ├── design_patterns.md      # Observer, RAII, Producer-Consumer
│   └── usage.md                # How to run & extend
└── yocto/                      # Yocto integration
    └── recipes-gateway/
        └── gateway/
            ├── gateway_1.0.bb  # Yocto recipe
            └── files/
                ├── gateway.service  # systemd unit
                └── default.conf
```

---

## Module Responsibilities

- **GatewayController** → Starts/stops all managers, coordinates lifecycle.  
- **SensorManager** → Polls sensors (UART/I²C/PWM), pushes data into `ThreadSafeQueue<SensorData>`.  
- **DataProcessor** → Consumes queue, applies filters/AI, forwards to CommManager + Logger.  
- **CommManager** → Publishes data via MQTT/TCP, handles retries, optional local TCP server.  
- **Logger** → Logs telemetry to CSV/JSON, rotates logs, supports log levels.  
- **OTAUpdater** → Async check for updates, downloads, validates, triggers system update.  
- **HAL (UARTPort, I2CDevice, PWMController, SocketClient)** → RAII wrappers for hardware access.  
- **Utils (MessageQueue, RingBuffer, RAIIHandle)** → Templates and resource management helpers.  
- **Tests** → GoogleTest unit tests with mocks for hardware.  

---

## Build & Run

### Build
#### Build MQTT Libs manually (Host pc)
    cd /MPU_Gateway
    mkdir -p build/External/paho.mqtt.c
    mkdir -p build/External/paho.mqtt.cpp

    cmake -S External/paho.mqtt.c -B build/External/paho.mqtt.c \
        -DPAHO_WITH_SSL=ON \
        -DPAHO_BUILD_STATIC=ON \
        -DPAHO_BUILD_SHARED=OFF \
        -DPAHO_ENABLE_CMAKE_CONFIG=ON

    cmake --build build/External/paho.mqtt.c --parallel

    cmake --install build/External/paho.mqtt.c --prefix External/paho.mqtt.c/install

    cmake -S External/paho.mqtt.cpp -B build/External/paho.mqtt.cpp \
        -DPAHO_BUILD_STATIC=ON \
        -DPAHO_BUILD_SHARED=OFF \
        -DPAHO_WITH_MQTT_C=OFF \
        -DCMAKE_PREFIX_PATH=$PWD/External/paho.mqtt.c/install

    cmake --build build/External/paho.mqtt.cpp --parallel

    cmake --install build/External/paho.mqtt.cpp --prefix External/paho.mqtt.cpp/install

#### CMD
    cd MPU_Gateway
    cmake --preset Debug (Release)
    cd build
    cmake ..
    make
#### VS Code
    cmake --preset Debug (Release)
    Build
    Run

### Run
```bash
./gateway --config ../configs/gateway_config.json
```

### Yocto Integration
- Add `yocto/recipes-gateway/` to your Yocto layer.
- Build image with:
```bash
bitbake core-image-minimal
```

---

## 📌 Development Roadmap (Follow During Production)

1. **Setup project skeleton** → CMake, folders, headers.  
2. **Implement HAL** → UART, I²C, PWM, Socket RAII wrappers.  
3. **Build SensorManager** → Poll sensors, push to queue.  
4. **Add DataProcessor** → Consume queue, filter/process data.  
5. **Implement CommManager** → TCP/IP + MQTT client.  
6. **Add Logger** → File I/O, operator overloading for `SensorData`.  
7. **Add OTAUpdater** → Async update checker.  
8. **Integrate GatewayController** → Orchestrate all modules.  
9. **Write Unit Tests** → GoogleTest with mocks.  
10. **Yocto Recipe** → Package app + systemd service.  
11. **Docs** → Architecture diagram, design patterns, usage guide.  

---

## 🎓 C++ Concepts Demonstrated
- **OOP**: Modular managers, inheritance for sensors.  
- **Multithreading**: Sensor threads, comm threads, OTA async.  
- **Templates**: Generic queues, buffers.  
- **RAII**: Resource wrappers for hardware.  
- **Smart Pointers**: Ownership management.  
- **Async**: OTA updates, logging.  
- **Operator Overloading**: Logging `SensorData`.  
- **Constexpr & Enums**: Compile-time constants, scoped IDs.  
- **Design Patterns**: Observer, Producer-Consumer, Singleton, RAII.  
