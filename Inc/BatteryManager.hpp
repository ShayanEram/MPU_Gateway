#ifndef BATTERYMANAGER_HPP
#define BATTERYMANAGER_HPP

#include <thread>
#include <atomic>
#include <iostream>
#include <fstream>

#include "InterData.hpp"

class BatteryManager {
public:
    explicit BatteryManager();
    ~BatteryManager();
    void start();
    void stop();

protected:

    
private:
    void runLoop();
    std::thread moduleThread;
    std::atomic<bool> running;

};

#endif // BATTERYMANAGER_HPP