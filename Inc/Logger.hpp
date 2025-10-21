#ifndef INC_LOGGER_HPP
#define INC_LOGGER_HPP

#include <string>
#include <ostream>

#include "DataManager.hpp"
#include "SensorManager.hpp"

class Logger {
public:
    explicit Logger(const std::string& path);

    void logProcessed(const ProcessedData& d);

    friend std::ostream& operator<<(std::ostream& os, const SensorData& s);

private:
    std::string _logPath;
};

std::ostream& operator<<(std::ostream& os, const SensorData& s);

#endif // INC_LOGGER_HPP