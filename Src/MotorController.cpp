#include "MotorController.hpp"

MotorController::MotorController() : running(false) {

}
MotorController::~MotorController() {
    stop();
}
//------------------------------------------------------------------------------------
void MotorController::start() {
    if (!running.load()) {
        running.store(true);
        moduleThread = std::thread(&MotorController::runLoop, this);
    }
}
void MotorController::stop() {
    if (running.load()) {
        running.store(false);
        if (moduleThread.joinable()) {
            moduleThread.join();
        }
    }
}
//------------------------------------------------------------------------------------
void MotorController::runLoop() {
    while (running.load()) {

        printf("Motor Controller is running.\n");
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
//------------------------------------------------------------------------------------
