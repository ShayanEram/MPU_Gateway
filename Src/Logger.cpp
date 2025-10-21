#include "Logger.hpp"

#include <fstream>
#include <filesystem>

Logger::Logger(const std::string& path) : _logPath(path) {
    if (!std::filesystem::exists(_logPath)) {
        std::ofstream ofs(_logPath); // create
    }
}
//------------------------------------------------------------------------------------
void Logger::logProcessed(const ProcessedData& d) {
    std::ofstream ofs(_logPath, std::ios::app);
    ofs << d.timestamp.time_since_epoch().count() << "," << d.value << ","
        << static_cast<int>(d.source) << "\n";
}

std::ostream& operator<<(std::ostream& os, const SensorData& s) {
    return os << s.timestamp.time_since_epoch().count() << "," << s.value << ","
              << static_cast<int>(s.type);
}