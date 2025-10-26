#include "DataManager.hpp"

#include <numeric>

DataManager::DataManager(std::size_t window) : _maxWindow(window) {
    // TODO: Preconfigure rules for each sensor type -> config file
    _rules[SensorType::UART]   = { .minDelta = 2.0,   .threshold = 0.0,  .period = std::chrono::seconds(10) };
    _rules[SensorType::I2C]    = { .minDelta = 0.0,   .threshold = 50.0, .period = std::chrono::seconds(60) };
    _rules[SensorType::SPI]    = { .minDelta = 0.5,   .threshold = 2.0,  .period = std::chrono::seconds(1) };
    _rules[SensorType::MODBUS] = { .minDelta = 1.0,   .threshold = 80.0, .period = std::chrono::seconds(60) };
    _rules[SensorType::OPCUA]  = { .minDelta = 5.0,   .threshold = 0.0,  .period = std::chrono::seconds(30) };
    _rules[SensorType::CAN]    = { .minDelta = 100.0, .threshold = 0.0,  .period = std::chrono::seconds(5) };
}

void DataManager::ingest(const SensorData& d) {
    _buffer.push_back(d);
    if (_buffer.size() > _maxWindow) {
        _buffer.pop_front();
    }
}

ProcessedData DataManager::filter() {
    if (_buffer.empty()) {
        return ProcessedData{};
    }

    const SensorData& data = _buffer.back();
    SensorState& state = _states[data.type];
    const FilterRule& rule = _rules[data.type];

    auto now = data.timestamp;

    // Time-based rule
    if (rule.period.count() > 0 && now - state.lastSent < rule.period){
        return {};
    }

    // Delta rule
    if (rule.minDelta > 0.0 && std::fabs(data.value - state.lastValue) < rule.minDelta){
        return {};
    }

    // Threshold rule
    if (rule.threshold > 0.0 && data.value < rule.threshold) {
        return {};
    }

    state.lastValue = data.value;
    state.lastSent = now;

    return {data.timestamp, data.value, data.type};
}

std::vector<ProcessedData> DataManager::batch(std::size_t n) const {
    std::vector<ProcessedData> out;
    out.reserve(n);
    std::size_t count = 0;
    for (auto it = _buffer.rbegin(); it != _buffer.rend() && count < n; ++it, ++count) {
        out.push_back(ProcessedData{it->timestamp, it->value, it->type});
    }
    std::reverse(out.begin(), out.end());
    return out;
}