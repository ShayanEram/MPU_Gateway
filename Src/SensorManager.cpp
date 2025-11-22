#include "SensorManager.hpp"

#include <cstdlib>
#include <ctime>
#include <string>

SensorManager::SensorManager(ThreadSafeQueue<SensorData>& q): _queue(q), _running(false), modbusClient_("/dev/ttyS0", 9600), i2cSensor_("/dev/i2c-1", 0x48),
                                                            spi_("/dev/spidev0.0", 500000, SPI_MODE_0, 8), opcuaClient_("opc.tcp://192.168.1.100:4840"),
                                                            modbusTcpClient_("192.168.1.50", 502), can_("can0"), pwm_(0, 0) 
{
    pwm_.setPeriod(20000000);     // 20 ms period
    pwm_.setDutyCycle(1500000);   // 1.5 ms pulse
    pwm_.enable();

}

SensorManager::~SensorManager() { 
    modbusClient_.disconnect();
    i2cSensor_.closeBus();
    spi_.closeBus();
    opcuaClient_.disconnect();
    modbusTcpClient_.disconnect();
    can_.closeBus();
    pwm_.disable();
    pwm_.unexportChannel();

    stop(); 
}
//------------------------------------------------------------------------------------
void SensorManager::start() {
    _running = true;
    _threads.emplace_back([this]() { pollUART(); });
    _threads.emplace_back([this]() { pollI2C(); });
    _threads.emplace_back([this]() { pollSPI(); });
    _threads.emplace_back([this]() { pollMODBUS(); });
    _threads.emplace_back([this]() { pollOPCUA(); });
    _threads.emplace_back([this]() { pollCAN(); });
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
static bool seeded = ([](){ std::srand(std::time(nullptr)); return true; })();
static double randInRange(double min, double max) {
    return min + (std::rand() / (double)RAND_MAX) * (max - min);
}

double SensorManager::readUARTSensor() {
    // Stub: simulate voltage reading
    return randInRange(220.0, 240.0); // volts
}

double SensorManager::readI2CSensor() {
    // Stub: simulate temperature
    return randInRange(25.0, 55.0); // °C
}

double SensorManager::readSPISensor() {
    // Stub: simulate RMS vibration
    return randInRange(0.5, 6.0); // mm/s
}

double SensorManager::readMODBUSSensor() {
    // Stub: simulate motor bearing temperature
    return randInRange(60.0, 95.0); // °C
}

double SensorManager::readOPCUASensor() {
    // Stub: simulate throughput metric
    return randInRange(100.0, 150.0); // units/min
}

double SensorManager::readCANSensor() {
    // Stub: simulate motor RPM
    return randInRange(1400.0, 1600.0); // RPM
}

#ifdef OPTIONAL_SENSORS
void SensorManager::writePWMSensor() {
    // Stub: simulate setting PWM duty cycle
    double duty = randInRange(0.0, 100.0);
    std::printf("PWM duty cycle set to %.1f%%\n", duty);
}
#endif
//------------------------------------------------------------------------------------
void SensorManager::pollUART() {
    while (_running) {
        SensorData d {
            std::chrono::system_clock::now(),
            readUARTSensor(),
            SensorType::UART
        };
        _queue.push(d);
        std::this_thread::sleep_for(std::chrono::milliseconds(UART_INTERVAL_CHECK_MS));
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
        std::this_thread::sleep_for(std::chrono::milliseconds(I2C_INTERVAL_CHECK_MS));
    }
}

void SensorManager::pollSPI() {
    while (_running) {
        SensorData d {
            std::chrono::system_clock::now(),
            readSPISensor(),
            SensorType::SPI
        };
        _queue.push(d);
        std::this_thread::sleep_for(std::chrono::milliseconds(SPI_INTERVAL_CHECK_MS));
    }
}

void SensorManager::pollMODBUS() {
    while (_running) {
        SensorData d {
            std::chrono::system_clock::now(),
            readMODBUSSensor(),
            SensorType::MODBUS
        };
        _queue.push(d);
        std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_INTERVAL_CHECK_MS));
    }
}

void SensorManager::pollOPCUA() {
    while (_running) {
        SensorData d {
            std::chrono::system_clock::now(),
            readOPCUASensor(),
            SensorType::OPCUA
        };
        _queue.push(d);
        std::this_thread::sleep_for(std::chrono::milliseconds(OPCUA_INTERVAL_CHECK_MS));
    }
}

void SensorManager::pollCAN() {
    while (_running) {
        SensorData d {
            std::chrono::system_clock::now(),
            readCANSensor(),
            SensorType::CAN
        };
        _queue.push(d);
        std::this_thread::sleep_for(std::chrono::milliseconds(CAN_INTERVAL_CHECK_MS));
    }
}