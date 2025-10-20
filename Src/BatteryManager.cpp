#include "BatteryManager.hpp"

BatteryManager::BatteryManager() : running(false) {

}

BatteryManager::~BatteryManager() {
    stop();
}
//------------------------------------------------------------------------------------
void BatteryManager::start() {
    if (!running.load()) {
        running.store(true);
        moduleThread = std::thread(&BatteryManager::runLoop, this);
    }
}

void BatteryManager::stop() {
    if (running.load()) {
        running.store(false);
        if (moduleThread.joinable()) {
            moduleThread.join();
        }
    }
}
//------------------------------------------------------------------------------------
void BatteryManager::runLoop() {
    while (running.load()) {

        printf("Battery Manager is running.\n");
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

    }
}
//------------------------------------------------------------------------------------
