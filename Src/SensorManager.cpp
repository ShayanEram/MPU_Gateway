#include "SensorManager.hpp"

SensorManager::SensorManager(ThreadSafeQueue<SensorData>& q): _queue(q), _running(false) {
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
    // TODO: returning stub
    return 42.0;
}

double SensorManager::readI2CSensor() {
    // TODO: returning stub
    return 24.0;
}

double SensorManager::readPWMSensor() {
    // TODO: returning stub
    return 0.5;
}
//------------------------------------------------------------------------------------
void SensorManager::pollUART() {
    while (_running) {
        SensorData d {
            std::chrono::system_clock::now(),
            readUARTSensor(),
            SensorType::UART
        };
        _queue.push(d);
        std::this_thread::sleep_for(std::chrono::milliseconds(UART_INTERVAL_CHECK));
    }
}

void SensorManager::pollI2C() {
    while (_running) {
        SensorData d {
            std::chrono::system_clock::now(),
            readI2CSensor(),
            SensorType::I2C
        };
        _queue.push(d);
        std::this_thread::sleep_for(std::chrono::milliseconds(I2C_INTERVAL_CHECK));
    }
}

void SensorManager::pollPWM() {
    while (_running) {
        SensorData d {
            std::chrono::system_clock::now(), 
            readPWMSensor(), 
            SensorType::PWM
        };
        _queue.push(d);
        std::this_thread::sleep_for(std::chrono::milliseconds(PWM_INTERVAL_CHECK));
    }
}