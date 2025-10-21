#include "DataManager.hpp"

#include <numeric>

void DataManager::ingest(const SensorData& d) {
    buffer.push_back(d);
    if (buffer.size() > maxWindow) {
        buffer.pop_front();
    }
}

ProcessedData DataManager::filter() const {
    if (buffer.empty()) {
        return ProcessedData{};
    }

    double sum = 0.0;

    for (const auto& s : buffer) {
        sum += s.value;
    }

    double avg = sum / static_cast<double>(buffer.size());

    ProcessedData pd{
        buffer.back().timestamp,
        avg,
        buffer.back().source
    };

    return pd;
}

std::vector<ProcessedData> DataManager::batch(std::size_t n) const {
    std::vector<ProcessedData> out;
    out.reserve(n);
    std::size_t count = 0;
    for (auto it = buffer.rbegin(); it != buffer.rend() && count < n; ++it, ++count) {
        out.push_back(ProcessedData{it->timestamp, it->value, it->source});
    }
    std::reverse(out.begin(), out.end());
    return out;
}