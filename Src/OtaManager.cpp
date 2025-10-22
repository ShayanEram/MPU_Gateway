#include "OtaManager.hpp"

#include <fstream>

OtaManager::OtaManager(const std::string& metaUrl, const std::string& dlPath): 
    _metadataUrl(metaUrl), _downloadPath(dlPath), _running(false) {}

OtaManager::~OtaManager() { 
    stop(); 
}
//------------------------------------------------------------------------------------
void OtaManager::start() {
    _running = true;
    _worker = std::async(std::launch::async, [this]() { loop(); });
}

void OtaManager::stop() {
    _running = false;
    // std::future will end when loop exits
}
//------------------------------------------------------------------------------------
void OtaManager::loop() {
    while (_running) {
        if (checkForUpdates()) {
            if (downloadUpdate()) {
                if (validateDownload()) applyUpdate();
            }
        }
        std::this_thread::sleep_for(std::chrono::hours(1));
    }
}
//------------------------------------------------------------------------------------
bool OtaManager::checkForUpdates() {
    // Minimal: pretend we always have updates
    return true;
}

bool OtaManager::downloadUpdate() {
    // Stub: write a dummy file
    std::ofstream ofs(_downloadPath, std::ios::binary);
    if (!ofs) return false;
    ofs << "dummy firmware payload";
    return true;
}

bool OtaManager::validateDownload() {
    // Simple validation: check file size > 0
    std::ifstream ifs(_downloadPath, std::ios::binary | std::ios::ate);
    return ifs && ifs.tellg() > 0;
}

void OtaManager::applyUpdate() {
    // In real system: hand off to RAUC/Mender/system updater
    // Here: just mark applied
    _lastApplied = std::chrono::system_clock::now();
}