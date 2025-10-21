#ifndef INC_CLOUDMANAGER_HPP_
#define INC_CLOUDMANAGER_HPP_

class CloudManager 
{
public:
    CloudManager() = default;
    ~CloudManager() = default;

    // Initialize cloud resources
    void init();

    // Poll/update cloud state; should be called periodically
    void update();

    // Query initialization state
    bool isInitialized() const noexcept { return initialized_; }

private:
    bool initialized_ = false;
};

#endif // INC_CLOUDMANAGER_HPP_