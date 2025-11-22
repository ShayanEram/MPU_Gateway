#ifndef INC_HW_HAL_HPP
#define INC_HW_HAL_HPP

#include <string>
#include <vector>
#include <cstdint>

#include <linux/spi/spidev.h>

class ModbusRTUClient {
public:
    ModbusRTUClient(const std::string& device, int baudrate);
    ~ModbusRTUClient();

    bool connect();
    void disconnect();

    // Read holding registers
    bool readHoldingRegisters(uint8_t slaveAddr, uint16_t startAddr, uint16_t count, std::vector<uint16_t>& outRegs);

private:
    int fd_;              // UART file descriptor
    std::string device_;  // e.g. "/dev/ttyS0"
    int baudrate_;

    bool sendRequest(const std::vector<uint8_t>& req);
    bool receiveResponse(std::vector<uint8_t>& resp);

    uint16_t crc16(const uint8_t* buf, int len);
};

class I2CDevice {
public:
    I2CDevice(const std::string& device, uint8_t address);
    ~I2CDevice();

    bool openBus();
    void closeBus();

    // Basic operations
    bool writeByte(uint8_t reg, uint8_t value);
    bool writeBytes(uint8_t reg, const std::vector<uint8_t>& values);

    bool readByte(uint8_t reg, uint8_t& value);
    bool readBytes(uint8_t reg, size_t count, std::vector<uint8_t>& values);

private:
    int fd_;                // File descriptor for /dev/i2c-X
    std::string device_;    // e.g. "/dev/i2c-1"
    uint8_t address_;       // I2C slave address
};

class SPIDevice {
public:
    SPIDevice(const std::string& device, uint32_t speedHz, uint8_t mode, uint8_t bitsPerWord);
    ~SPIDevice();

    bool openBus();
    void closeBus();

    // Transfer data (full-duplex)
    bool transfer(const std::vector<uint8_t>& tx, std::vector<uint8_t>& rx);

    // Convenience: write only
    bool writeBytes(const std::vector<uint8_t>& tx);

    // Convenience: read only (send dummy bytes)
    bool readBytes(size_t count, std::vector<uint8_t>& rx);

private:
    int fd_;                // File descriptor for /dev/spidevX.Y
    std::string device_;    // e.g. "/dev/spidev0.0"
    uint32_t speedHz_;
    uint8_t mode_;
    uint8_t bitsPerWord_;
};

class OPCUAClient {
public:
    OPCUAClient(const std::string& endpointUrl);
    ~OPCUAClient();

    bool connect();
    void disconnect();

    // Read a variable node
    bool readValue(const std::string& nodeId, double& outValue);

    // Write a variable node
    bool writeValue(const std::string& nodeId, double value);

    // Publish sensor data (helper)
    bool publishSensorData(const std::string& nodeId, const std::vector<double>& values);

private:
    std::string endpointUrl_;
    void* client_; // opaque pointer to open62541 client
};

class ModbusTCPClient {
public:
    ModbusTCPClient(const std::string& ip, int port);
    ~ModbusTCPClient();

    bool connect();
    void disconnect();

    // Read holding registers
    bool readHoldingRegisters(int addr, int nb, std::vector<uint16_t>& outRegs);

    // Write single register
    bool writeRegister(int addr, uint16_t value);

    // Write multiple registers
    bool writeRegisters(int addr, const std::vector<uint16_t>& values);

private:
    std::string ip_;
    int port_;
    void* ctx_; // opaque pointer to libmodbus context
};

struct CANFrame {
    uint32_t id;       // CAN identifier
    uint8_t dlc;       // Data length code (0–8)
    uint8_t data[8];   // Payload
};

class CANDevice {
public:
    CANDevice(const std::string& ifname);
    ~CANDevice();

    bool openBus();
    void closeBus();

    bool sendFrame(const CANFrame& frame);
    bool receiveFrame(CANFrame& frame);

private:
    std::string ifname_;
    int fd_; // socket file descriptor
};

class PWMChannel {
public:
    PWMChannel(int chip, int channel);
    ~PWMChannel();

    bool exportChannel();
    void unexportChannel();

    bool enable();
    void disable();

    bool setPeriod(unsigned int nanoseconds);
    bool setDutyCycle(unsigned int nanoseconds);

private:
    int chip_;      // PWM chip index (pwmchipN)
    int channel_;   // PWM channel index (pwmN)
    std::string basePath_; // sysfs path
};

#endif // INC_HW_HAL_HPP