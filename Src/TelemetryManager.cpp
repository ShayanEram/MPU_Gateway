#include "TelemetryManager.hpp"

TelemetryManager::TelemetryManager() : running(false) {
   
}
TelemetryManager::~TelemetryManager() {
    stop();
}
//------------------------------------------------------------------------------------
void TelemetryManager::start() {
    if (!running.load()) {
        running.store(true);
        moduleThread = std::thread(&TelemetryManager::runLoop, this);
    }
}
void TelemetryManager::stop() {
    if (running.load()) {
        running.store(false);
        if (moduleThread.joinable()) {
            moduleThread.join();
        }
    }
}
//------------------------------------------------------------------------------------
void TelemetryManager::runLoop() {
    while (running.load()) {

        printf("Telemetry Manager is running.\n");
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

    }
}
//------------------------------------------------------------------------------------
