#include "GatewayController.hpp"

#include <thread>
#include <chrono>
#include <iostream>

GatewayController::GatewayController(const std::string& cfg)
    : _sensorQueue(),
      _protocolManager(),
      _dataManager(),
      _cloudManager("tcp://broker.hivemq.com:1883", "gateway-001"),
      _logger("/var/log/gateway.log"),
      _deviceManager(cfg),
      _dataProcessor(_sensorQueue, _dataManager, _protocolManager, _cloudManager, _logger),
      _otaManager("http://updates.example/meta", "/tmp/firmware.bin"),
      _running(false)
{
    _deviceManager.loadConfig(cfg);
    _deviceManager.subscribe([this](const ConfigMap& cfg){
        // Apply updated settings to subsystems (sampling rates, endpoints, etc.)
        (void)cfg;
    });
}

GatewayController::~GatewayController() { 
    stop();
}

void GatewayController::start() {
    _running = true;
    _sensorManager = std::make_unique<SensorManager>(_sensorQueue);
    _sensorManager->start();
    _dataProcessor.start();
    _otaManager.start();

    _cloudManager.setCommandHandler([](const std::string& topic, const std::string& payload) {
        std::cout << "[App] Command received on " << topic
                  << " with payload: " << payload << std::endl;
        // TODO: Dispatch to device logic here
    });
    _cloudManager.connect();
    _cloudManager.subscribe("gateway/001/commands/#");

    while (_running) {
        auto health = _deviceManager.snapshotHealth();
        // Optionally publish health or log
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}

void GatewayController::stop() {
    if (!_running) {
        return;
    }

    _running = false;

    if (_sensorManager) {
        _sensorManager->stop();
    }
    _dataProcessor.stop();
    _otaManager.stop();
}