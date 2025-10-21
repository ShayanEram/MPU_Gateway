#ifndef INC_DATAMANAGER_HPP
#define INC_DATAMANAGER_HPP

#include <vector>
#include <deque>

#include "SensorManager.hpp"

struct ProcessedData 
{
    std::chrono::system_clock::time_point timestamp;
    double value;
    SensorType source;
};

class DataManager 
{
public:
    DataManager() = default;
    ~DataManager() = default;

    void ingest(const SensorData& d);
    ProcessedData filter() const;
    std::vector<ProcessedData> batch(std::size_t n) const;

private:
    std::deque<SensorData> buffer;
    std::size_t maxWindow;

};

#endif // INC_DATAMANAGER_HPP