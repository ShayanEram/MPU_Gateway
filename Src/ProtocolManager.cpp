#include "ProtocolManager.hpp"

ProtocolManager::ProtocolManager(const std::string& baseTopic)
    : _baseTopic(baseTopic) {}
//------------------------------------------------------------------------------------
std::string ProtocolManager::toCloudPayload(const ProcessedData& pd) const {
    json j;
    j["timestamp"] = std::chrono::duration_cast<std::chrono::milliseconds>(pd.timestamp.time_since_epoch()).count();
    j["value"] = pd.value;
    j["type"] = sensorTypeToString(pd.source);
    return j.dump(4);
}

Command ProtocolManager::fromCloudPayload(const std::string& payload) const {
    auto j = json::parse(payload);
    Command cmd;

    auto getCmdType = [](const std::string& Icmd){
        switch(Icmd[0])
        {
            case 'N':
                return CommandType::None;
            case 'R':
                return CommandType::Restart;
            case 'S':
                return CommandType::Shutdown;
            case 'C':
                return CommandType::ReloadConfig;
            default:
                return CommandType::None;
        }
    };
    
    cmd.command = j.value("command", "");
    cmd.parameter = j.value("parameter", 0.0);
    cmd.type = getCmdType(cmd.command);
    return cmd;
}
//------------------------------------------------------------------------------------
std::string ProtocolManager::sensorTypeToString(SensorType t) const {
    switch (t) {
        case SensorType::UART:   return "uart";
        case SensorType::I2C:    return "i2c";
        case SensorType::SPI:    return "spi";
        case SensorType::MODBUS: return "modbus";
        case SensorType::OPCUA:  return "opcua";
        case SensorType::CAN:    return "can";
        default:                 return "unknown";
    }
}