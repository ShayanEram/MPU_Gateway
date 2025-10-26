/**
 * SensorManager collects values from sensors and sends them to other modules using a Queue.
 */
#ifndef INC_SENSORMANAGER_HPP
#define INC_SENSORMANAGER_HPP

#include <vector>
#include <thread>
#include <atomic>
#include <chrono>

#include "ThreadSafeQueue.hpp"

enum class SensorType 
{
    UART,
    I2C,
    PWM
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
    double readUARTSensor();
    double readI2CSensor();
    double readPWMSensor();
    void pollUART();
    void pollI2C();
    void pollPWM();

    static constexpr int UART_INTERVAL_CHECK = 100;
    static constexpr int I2C_INTERVAL_CHECK = 200;
    static constexpr int PWM_INTERVAL_CHECK = 300;

    std::atomic<bool> _running;
    std::vector<std::thread> _threads;
    ThreadSafeQueue<SensorData>& _queue;
};

#endif // INC_SENSORMANAGER_HPP