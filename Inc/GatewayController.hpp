#ifndef INC_GATEWAYCONTROLLER_HPP
#define INC_GATEWAYCONTROLLER_HPP

#include <memory>
#include <atomic>

#include "SensorManager.hpp"
#include "DataProcessor.hpp"
#include "ProtocolManager.hpp"
#include "DataManager.hpp"
#include "CloudManager.hpp"
#include "DeviceManager.hpp"
#include "OtaManager.hpp"
#include "Logger.hpp"
#include "ThreadSafeQueue.hpp"

class GatewayController {
public:
    GatewayController(const std::string& cfg);
    ~GatewayController();

    void start();
    void stop();

private:

    ThreadSafeQueue<SensorData> _sensorQueue;

    ProtocolManager _protocolManager;
    DataManager     _dataManager;
    CloudManager    _cloudManager;
    Logger          _logger;
    DeviceManager   _deviceManager;
    DataProcessor   _dataProcessor;
    OtaManager      _otaManager;

    std::unique_ptr<SensorManager> _sensorManager;
    std::atomic<bool> _running;

};

#endif // INC_GATEWAYCONTROLLER_HPP