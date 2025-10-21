#ifndef INC_SENSORMANAGER_HPP
#define INC_SENSORMANAGER_HPP

#include "ThreadSafeQueue.hpp"

class SensorManager {
public:
    SensorManager() = default;
    ~SensorManager() = default;

    void start();
    void stop();

    double readUARTSensor();
    double readI2CSensor();
    double readPWMSensor();

    void pollUART();
    void pollI2C();
    void pollPWM();

private:

};

#endif // INC_SENSORMANAGER_HPP