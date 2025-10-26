/**
 * DataManager is tasked to filter out sensors data to send to the cloud
 */
#ifndef INC_DATAMANAGER_HPP
#define INC_DATAMANAGER_HPP

#include "SensorManager.hpp"

#include <deque>
#include <unordered_map>
#include <chrono>
#include <cmath>
#include <vector>
#include <algorithm>

struct ProcessedData 
{
    std::chrono::system_clock::time_point timestamp;
    double value;
    SensorType source;
};

struct FilterRule {
    double minDelta = 0.0;
    double threshold = 0.0;
    std::chrono::seconds period;
    bool average = false;
    bool rms = false;
    bool onChangeOnly = false;
};

struct SensorState {
    double lastValue = 0.0;
    std::chrono::system_clock::time_point lastSent;
};

class DataManager 
{
public:
    explicit DataManager(std::size_t window = 10);
    ~DataManager() = default;

    void ingest(const SensorData& d);
    ProcessedData filter();
    std::vector<ProcessedData> batch(std::size_t n) const;

private:
    std::deque<SensorData> _buffer; // Add value to the front and disgard old ones
    std::size_t _maxWindow;
    std::unordered_map<SensorType, FilterRule> _rules;
    std::unordered_map<SensorType, SensorState> _states;
};

#endif // INC_DATAMANAGER_HPP