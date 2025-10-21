#include "SensorManager.hpp"

SensorManager::SensorManager(): _running(false) {
    // Constructor implementation
}

SensorManager::~SensorManager() { 
    stop(); 
}
//------------------------------------------------------------------------------------
void SensorManager::start() {
    _running = true;
    _threads.emplace_back([this]() { pollUART(); });
    _threads.emplace_back([this]() { pollI2C(); });
    _threads.emplace_back([this]() { pollPWM(); });
}

void SensorManager::stop() {
    _running = false;
    for (auto& t : _threads) {
        if (t.joinable()) {
            t.join();
        }
    }
}
//------------------------------------------------------------------------------------
double SensorManager::readUARTSensor() {
    // TODO: read from UARTPort HAL; returning stub
    return 42.0;
}

double SensorManager::readI2CSensor() {
    // TODO: read from I2CDevice HAL; returning stub
    return 24.0;
}

double SensorManager::readPWMSensor() {
    // TODO: read from PWM controller or measure duty; returning stub
    return 0.5;
}
//------------------------------------------------------------------------------------
void SensorManager::pollUART() {
    while (_running) {
        SensorData d{std::chrono::system_clock::now(), readUARTSensor(), SensorType::UART};
        _queue.push(d);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void SensorManager::pollI2C() {
    while (_running) {
        SensorData d{std::chrono::system_clock::now(), readI2CSensor(), SensorType::I2C};
        _queue.push(d);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}

void SensorManager::pollPWM() {
    while (_running) {
        SensorData d{std::chrono::system_clock::now(), readPWMSensor(), SensorType::PWM};
        _queue.push(d);
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
}