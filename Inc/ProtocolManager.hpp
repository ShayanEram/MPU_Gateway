#ifndef INC_PROTOCOLMANAGER_HPP
#define INC_PROTOCOLMANAGER_HPP

#include <string>
#include <sstream>

#include "DataManager.hpp"

enum class ProtocolType 
{
    MQTT,
    HTTP,
    CoAP
};

enum class EncodingType 
{
    JSON,
    XML,
    CBOR
};

enum class CommandType 
{
    None,
    Restart,
    Shutdown,
    ReloadConfig
};

struct Command  
{
    // ProtocolType protocol;
    // EncodingType encoding;
    CommandType type;
};

class ProtocolManager {
public:
    ProtocolManager() = default;
    ~ProtocolManager() = default;

    std::string toCloudPayload(const ProcessedData& pd) const;
    Command fromCloudPayload(const std::string& payload) const;
};

#endif // INC_PROTOCOLMANAGER_HPP