#ifndef INC_LOGGER_HPP
#define INC_LOGGER_HPP

#include <string>
#include <ostream>

#include "DataManager.hpp"
#include "SensorManager.hpp"

class Logger {
public:
    explicit Logger(const std::string& path);
    ~Logger() = default;

    void logProcessed(const ProcessedData& d);

private:
    std::string _logPath;
};

// SensorData
std::ostream& operator<<(std::ostream& os, const SensorData& s);

#endif // INC_LOGGER_HPP