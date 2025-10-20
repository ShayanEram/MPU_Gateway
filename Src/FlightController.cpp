#include "FlightController.hpp"

FlightController::FlightController() : running(false)
{

}
FlightController::~FlightController() {
    stop();
}
//------------------------------------------------------------------------------------
void FlightController::start() {
    if (!running.load()) {
        running.store(true);
        moduleThread = std::thread(&FlightController::runLoop, this);
    }
}
void FlightController::stop() {
    if (running.load()) {
        running.store(false);
        if (moduleThread.joinable()) {
            moduleThread.join();
        }
    }
}
//------------------------------------------------------------------------------------
void FlightController::runLoop() {

    while (running.load()) 
    {
        printf("Flight Controller is running.\n");
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

    }
}
//------------------------------------------------------------------------------------
