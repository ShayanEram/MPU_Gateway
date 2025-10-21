#ifndef INC_OTAMANAGER_HPP
#define INC_OTAMANAGER_HPP

class OtaManager {
public:
    OtaManager() = default;
    ~OtaManager() = default;

    // Initialize OTA manager resources
    void init();

    // Start the OTA update process
    void startUpdate();

    // Query initialization state
    bool isInitialized() const noexcept { return initialized_; }

private:
    bool initialized_ = false;
};

#endif // INC_OTAMANAGER_HPP