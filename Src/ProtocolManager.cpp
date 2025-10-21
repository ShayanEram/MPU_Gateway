#include "ProtocolManager.hpp"

std::string ProtocolManager::toCloudPayload(const ProcessedData& pd) const {
    // CSV payload: ts,value,source
    std::ostringstream oss;
    oss << pd.timestamp.time_since_epoch().count() << ","
        << pd.value << ","
        << static_cast<int>(pd.source);
    return oss.str();
}

Command ProtocolManager::fromCloudPayload(const std::string& msg) const {
    // key=value parser: cmd=restart or config=reload
    Command cmd{CommandType::None};
    auto pos = msg.find('=');
    if (pos != std::string::npos) {
        std::string key = msg.substr(0, pos);
        std::string val = msg.substr(pos + 1);
        if (key == "cmd" && val == "restart") cmd.type = CommandType::Restart;
        else if (key == "cmd" && val == "shutdown") cmd.type = CommandType::Shutdown;
        else if (key == "config" && val == "reload") cmd.type = CommandType::ReloadConfig;
    }
    return cmd;
}