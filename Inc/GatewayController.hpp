#ifndef INC_GATEWAYCONTROLLER_HPP
#define INC_GATEWAYCONTROLLER_HPP

class GatewayController {
public:
    GatewayController() = default;
    ~GatewayController() = default;

    // Initialize gateway resources
    void init();

    // Poll/update gateway state; should be called periodically
    void update();

    // Query initialization state
    bool isInitialized() const noexcept { return initialized_; }

private:
    bool initialized_ = false;
};

#endif // INC_GATEWAYCONTROLLER_HPP