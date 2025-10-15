# Industrial IoT Gateway (C++23 + Yocto)

## 📌 Overview
This project implements an **Industrial IoT Gateway** in **modern C++23**, designed to run on an embedded Linux system built with **Yocto**.  

The gateway collects data from sensors (via **UART, I²C, PWM**), processes it locally, and publishes telemetry to the cloud using **TCP/IP and MQTT**. It demonstrates **production-grade architecture** and advanced C++ concepts, making it both **industry-relevant** and **resume-ready**.

---

## 🎯 Features
- **Multithreaded data acquisition** from multiple hardware interfaces.
- **Asynchronous networking** (TCP/IP, MQTT) for cloud communication.
- **Object-Oriented modular design** with clear separation of concerns.
- **RAII-based resource management** for hardware and system resources.
- **Templated data structures** (`MessageQueue<T>`, `RingBuffer<T>`).
- **Producer-Consumer pipelines** for inter-thread communication.
- **Observer pattern** for event-driven updates.
- **File I/O** for telemetry logging and configuration parsing.
- **OTA update checker** with async execution.
- **Yocto integration** with a custom recipe and systemd service.

---

## 🧩 Project Structure

```
gateway-project/
├── CMakeLists.txt              # Top-level build config
├── cmake/                      # Toolchain & cross-compilation configs
├── include/                    # Public headers
│   ├── gateway/
│   │   ├── GatewayController.hpp
│   │   ├── SensorManager.hpp
│   │   ├── CommManager.hpp
│   │   ├── DataProcessor.hpp
│   │   ├── Logger.hpp
│   │   ├── OTAUpdater.hpp
│   │   └── utils/
│   │       ├── MessageQueue.hpp
│   │       ├── RingBuffer.hpp
│   │       ├── RAIIHandle.hpp
│   │       └── ThreadSafeQueue.hpp
├── src/                        # Implementation files
│   ├── main.cpp
│   ├── GatewayController.cpp
│   ├── SensorManager.cpp
│   ├── CommManager.cpp
│   ├── DataProcessor.cpp
│   ├── Logger.cpp
│   ├── OTAUpdater.cpp
│   └── hw/                     # Hardware abstraction layer
│       ├── UARTPort.cpp
│       ├── I2CDevice.cpp
│       ├── PWMController.cpp
│       └── SocketClient.cpp
├── tests/                      # Unit & integration tests
│   ├── test_SensorManager.cpp
│   ├── test_CommManager.cpp
│   ├── test_DataProcessor.cpp
│   └── mocks/
│       ├── MockUART.hpp
│       ├── MockI2C.hpp
│       └── MockSocket.hpp
├── configs/                    # Config files
│   ├── gateway_config.json
│   └── logging.conf
├── scripts/                    # Helper scripts
│   ├── run_gateway.sh
│   └── ota_update.sh
├── docs/                       # Documentation
│   ├── architecture.md
│   ├── design_patterns.md
│   └── usage.md
└── yocto/                      # Yocto integration
    ├── recipes-gateway/
    │   ├── gateway/
    │   │   ├── gateway_1.0.bb
    │   │   └── files/
    │   │       ├── gateway.service
    │   │       └── default.conf
```

---

## 🧠 C++ Concepts Demonstrated

- **OOP**: Modular classes (`SensorManager`, `CommManager`, etc.) with encapsulation, inheritance, and composition.  
- **Multithreading**: `std::thread`, `std::mutex`, `std::condition_variable` for concurrent tasks.  
- **Templates**: Generic `MessageQueue<T>`, `RingBuffer<T>`, `Observer<T>`.  
- **RAII & Smart Pointers**: Resource wrappers for UART/I²C, `std::unique_ptr`, `std::shared_ptr`.  
- **Async Programming**: `std::future`, `std::async` for OTA updates and logging.  
- **Operator Overloading**: `<<` for `SensorData` logging.  
- **Constexpr & Enums**: Compile-time constants and scoped enums for protocol IDs.  
- **Design Patterns**: Observer, Producer-Consumer, Singleton, RAII.  
- **File I/O**: Config parsing (JSON), telemetry logging (CSV/JSON).  
- **Networking**: TCP/IP client, MQTT publish/subscribe.  
- **Testing**: GoogleTest with mocks for hardware abstraction.  

---

## 🚀 Build & Run

### Build
```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

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
