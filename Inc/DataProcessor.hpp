#ifndef INC_DATAPROCESSOR_HPP
#define INC_DATAPROCESSOR_HPP

#include <atomic>
#include <thread>

#include "ThreadSafeQueue.hpp"
#include "SensorManager.hpp"
#include "DataManager.hpp"
#include "ProtocolManager.hpp"
#include "CloudManager.hpp"
#include "Logger.hpp"

class DataProcessor {
public:
    DataProcessor();
    ~DataProcessor();

    void start();
    void stop();

private:
    void loop();

    std::thread _worker;
    std::atomic<bool> _running;

    ThreadSafeQueue<SensorData> _inQueue;
    DataManager _dataManager;
    ProtocolManager _protocol;
    CloudManager _cloud;
    Logger _logger;
    
};

#endif // INC_DATAPROCESSOR_HPP