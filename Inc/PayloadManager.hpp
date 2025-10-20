/**
 * @file PayloadManager.hpp
 * @brief Header file for the PayloadManager class.
 * This module ensures proper handling and deployment of payloads.
 */

#ifndef PAYLOADMANAGER_HPP
#define PAYLOADMANAGER_HPP

#include <thread>
#include <atomic>
#include <iostream>
#include <fstream>

#include "InterData.hpp"

class PayloadManager {
public:
    explicit PayloadManager();
    ~PayloadManager();
    void start();
    void stop();

protected:


private:
    void runLoop();
    std::thread moduleThread;
    std::atomic<bool> running;

};

#endif // PAYLOADMANAGER_HPP