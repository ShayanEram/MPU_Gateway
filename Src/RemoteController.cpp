#include "RemoteController.hpp"
#include <string>

RemoteController::RemoteController() : running(false) {

}
RemoteController::~RemoteController() {
    stop();
}
//------------------------------------------------------------------------------------
void RemoteController::start() {
    if (!running.load()) {
        running.store(true);
        moduleThread = std::thread(&RemoteController::runLoop, this);
    }
}
void RemoteController::stop() {
    if (running.load()) {
        running.store(false);
        if (moduleThread.joinable()) {
            moduleThread.join();
        }
    }
}
//------------------------------------------------------------------------------------
void RemoteController::runLoop() {

    while (running.load()) 
    {
        printf("Remote Controller is running.\n");
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

}
//------------------------------------------------------------------------------------
