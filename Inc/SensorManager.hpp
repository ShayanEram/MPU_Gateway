#ifndef INC_SENSORMANAGER_HPP
#define INC_SENSORMANAGER_HPP

class SensorManager {
public:
    SensorManager() = default;
    ~SensorManager() = default;

    // Initialize sensors and resources
    void init();

    // Poll/update sensor state; should be called periodically
    void update();

    // Query initialization state
    bool isInitialized() const noexcept { return initialized_; }

private:
    bool initialized_ = false;
};

#endif // INC_SENSORMANAGER_HPP