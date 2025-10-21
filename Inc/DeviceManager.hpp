#ifndef INC_DEVICEMANAGER_HPP
#define INC_DEVICEMANAGER_HPP

#include <functional>
#include <string>

class DeviceManager {
public:
    DeviceManager() = default;
    ~DeviceManager() = default;

    DeviceManager(const DeviceManager&) = delete;
    DeviceManager& operator=(const DeviceManager&) = delete;

    bool loadConfig(const std::string& path);
    void applyCommand();
    void snapshotHealth() const;
    void notifyObservers();
    void subscribe(std::function<void(const int&)> cb);
};

#endif // INC_DEVICEMANAGER_HPP