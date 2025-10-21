#ifndef INC_PROTOCOLMANAGER_HPP
#define INC_PROTOCOLMANAGER_HPP

#include <sstream>

class ProtocolManager {
public:
    ProtocolManager() = default;
    ~ProtocolManager() = default;

    std::string toCloudPayload() const;
    void fromCloudPayload(const std::string& payload) const;

};

#endif // INC_PROTOCOLMANAGER_HPP