#ifndef INC_DATAMANAGER_HPP
#define INC_DATAMANAGER_HPP

class DataManager 
{
public:
    DataManager() = default;
    ~DataManager() = default;

    // Initialize data resources
    void init();

    // Poll/update data state; should be called periodically
    void update();

    // Query initialization state
    bool isInitialized() const noexcept { return initialized_; }

private:
    bool initialized_ = false;
};

#endif // INC_DATAMANAGER_HPP