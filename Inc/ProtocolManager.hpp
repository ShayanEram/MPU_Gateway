/**
 * ProtocolManager: Translate each data to the right protocol for the cloud.
 * The protocol used is MQTT with JSON
 */
#ifndef INC_PROTOCOLMANAGER_HPP
#define INC_PROTOCOLMANAGER_HPP

#include <string>
#include <json.hpp>

#include "DataManager.hpp"

using json = nlohmann::json;

enum class CommandType 
{
    None,
    Restart,
    Shutdown,
    ReloadConfig
};

struct Command   
{
    CommandType type;
    std::string command;
    double parameter;
};

class ProtocolManager {
public:
    ProtocolManager(const std::string& baseTopic = "factory/gateway");
    ~ProtocolManager() = default;

    std::string toCloudPayload(const ProcessedData& pd) const;
    Command fromCloudPayload(const std::string& payload) const;

private:
    std::string sensorTypeToString(SensorType t) const;
    std::string _baseTopic;
};

#endif // INC_PROTOCOLMANAGER_HPP