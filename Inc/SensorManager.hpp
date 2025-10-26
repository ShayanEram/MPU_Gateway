/**
 * SensorManager collects values from sensors and sends them to other modules using a Queue.
 */
#ifndef INC_SENSORMANAGER_HPP
#define INC_SENSORMANAGER_HPP

#define OPTIONAL_SENSORS

#include <vector>
#include <thread>
#include <atomic>
#include <chrono>

#include "ThreadSafeQueue.hpp"

enum class SensorType 
{
    UART,
    I2C,
    SPI,
    MODBUS,
    OPCUA,
    CAN
    #ifdef OPTIONAL_SENSORS
        ,PWM
    #endif
};

struct SensorData 
{
    std::chrono::system_clock::time_point timestamp;
    double value;
    SensorType type;
};

class SensorManager {
public:
    SensorManager(ThreadSafeQueue<SensorData>& q);
    ~SensorManager();
    void start();
    void stop();

private:
    /**
     * UART → Modbus RTU (over RS‑485)
     * 
     * Connected to: Power meter (measures current, voltage, power factor).
     * Filtering rules: Voltage/current: report every 10 s, but only if change > 2%.
     */
    double readUARTSensor();
    /**
     * I²C (on-board)
     * 
     * Connected to: Internal gateway temperature/humidity sensor.
     * Filtering rules: Report every 60 s. Only send if > 50 °C (overheating).
     */
    double readI2CSensor();
    /**
     * SPI
     * 
     * Connected to: Optional vibration sensor module (high-speed IMU).
     * Filtering rules: Compute RMS vibration over 1 s window.
     */
    double readSPISensor();
    /**
     * Modbus TCP (Ethernet)
     * 
     * Connected to: Temperature sensors on motors and bearings.
     * Filtering rules: Average temperature over 1 min.
     */
    double readMODBUSSensor();
    /**
     * OPC‑UA (Ethernet)
     * 
     * Connected to: PLC controlling a conveyor line (exposes tags for machine state).
     * Filtering rules: 
     */
    double readOPCUASensor();
    /**
     * CAN
     * 
     * Connected to: Motor controller (reports RPM).
     * Filtering rules: send every 5 s, or if change > 100 RPM.
     */
    double readCANSensor();

    #ifdef OPTIONAL_SENSORS
        /**
         * PWM
         * 
         * Connected to (optional): Local fan or valve actuator.
         * Filtering rules: N/A, PWM is output!
         */
        void writePWMSensor();
    #endif
    
    void pollUART();
    void pollI2C();
    void pollSPI();
    void pollMODBUS();
    void pollOPCUA();
    void pollCAN();

    static constexpr int UART_INTERVAL_CHECK_MS   = 100;
    static constexpr int I2C_INTERVAL_CHECK_MS    = 200;
    static constexpr int SPI_INTERVAL_CHECK_MS    = 300;
    static constexpr int MODBUS_INTERVAL_CHECK_MS = 100;
    static constexpr int OPCUA_INTERVAL_CHECK_MS  = 200;
    static constexpr int CAN_INTERVAL_CHECK_MS    = 300;

    std::atomic<bool> _running;
    std::vector<std::thread> _threads;
    ThreadSafeQueue<SensorData>& _queue;
};

#endif // INC_SENSORMANAGER_HPP