#ifndef INC_DEVICEMANAGER_HPP
#define INC_DEVICEMANAGER_HPP

class DeviceManager {
public:
    DeviceManager() = default;
    ~DeviceManager() = default;

    DeviceManager(const DeviceManager&) = delete;
    DeviceManager& operator=(const DeviceManager&) = delete;

    // Initialize device manager resources
    void initialize();

    // Clean up resources
    void shutdown();
};

#endif // INC_DEVICEMANAGER_HPP