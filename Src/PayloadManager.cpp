#include "PayloadManager.hpp"

PayloadManager::PayloadManager() : running(false) {

}

PayloadManager::~PayloadManager() {
    stop();
}
//------------------------------------------------------------------------------------
void PayloadManager::start() {
    if (!running.load()) {
        running.store(true);
        moduleThread = std::thread(&PayloadManager::runLoop, this);
    }
}
void PayloadManager::stop() {
    if (running.load()) {
        running.store(false);
        if (moduleThread.joinable()) {
            moduleThread.join();
        }
    }
}
//------------------------------------------------------------------------------------
void PayloadManager::runLoop() {
    while (running.load()) 
    {
        printf("Payload Manager is running.\n");
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
//------------------------------------------------------------------------------------
