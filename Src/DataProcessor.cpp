#include "DataProcessor.hpp"

#include <numeric>
#include <algorithm>

DataProcessor::DataProcessor(): _running(false) {
    // Constructor implementation
}

DataProcessor::~DataProcessor() { 
    stop(); 
}
//------------------------------------------------------------------------------------
void DataProcessor::start() {
    _running = true;
    _worker = std::thread([this]() { loop(); });
}

void DataProcessor::stop() {
    _running = false;
    if (_worker.joinable()) _worker.join();
}
//------------------------------------------------------------------------------------
void DataProcessor::loop() {
    while (_running) {
        SensorData d;
        if (_inQueue.pop(d)) {
            _dataManager.ingest(d);                  // aggregate
            ProcessedData pd = _dataManager.filter(); // filtered window result
            std::string payload = _protocol.toCloudPayload(pd); // translate
            _cloud.publish(payload);                 // send upstream
            _logger.logProcessed(pd);                // persist locally
        }
    }
}