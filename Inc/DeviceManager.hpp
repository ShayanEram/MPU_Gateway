#ifndef INC_DEVICEMANAGER_HPP
#define INC_DEVICEMANAGER_HPP

#include <functional>
#include <string>
#include <map>
#include <vector>

#include "ProtocolManager.hpp"

using ConfigMap = std::map<std::string, std::string>;

struct DeviceHealth {
    long uptimeSeconds{};
    long totalRamKB{};
    long freeRamKB{};
};

class DeviceManager {
public:
    explicit DeviceManager() = default;
    ~DeviceManager() = default;

    DeviceManager(const DeviceManager&) = delete;
    DeviceManager& operator=(const DeviceManager&) = delete;

    bool loadConfig(const std::string& path);
    void applyCommand(Command cmd);
    DeviceHealth snapshotHealth() const;

    void subscribe(std::function<void(const ConfigMap&)> cb);

private:
    void notifyObservers();
    std::string _configPath;
    ConfigMap _configData;
    std::vector<std::function<void(const ConfigMap&)>> _observers;

};

#endif // INC_DEVICEMANAGER_HPP