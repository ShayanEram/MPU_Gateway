#include "DeviceManager.hpp"

#include <fstream>
#include <sstream>
#include <sys/sysinfo.h>

DeviceManager::DeviceManager(const std::string& cfgPath) 
    : _configPath(cfgPath) {}

//------------------------------------------------------------------------------------
bool DeviceManager::loadConfig(const std::string& path) {
    std::ifstream ifs(path);
    if (!ifs) 
        return false;
    _configData.clear();
    std::string line;

    while (std::getline(ifs, line)) {
        auto pos = line.find('=');
        if (pos == std::string::npos) 
            continue;
        _configData[line.substr(0, pos)] = line.substr(pos + 1);
    }
    return true;
}

void DeviceManager::applyCommand(Command cmd) {
    switch (cmd.type) {
        case CommandType::ReloadConfig:
            loadConfig(_configPath);
            notifyObservers();
            break;
        case CommandType::Restart:
            std::exit(42); // systemd Restart=on-failure will relaunch
            break;
        case CommandType::Shutdown:
            std::system("systemctl poweroff");
            break;
        default: break;
    }
}

DeviceHealth DeviceManager::snapshotHealth() const {
    struct sysinfo info{}; // From linux kernel values
    sysinfo(&info);
    
    DeviceHealth h{};
    h.uptimeSeconds = info.uptime;
    h.totalRamKB = info.totalram / 1024;
    h.freeRamKB  = info.freeram  / 1024;
    return h;
}

void DeviceManager::notifyObservers() {
    for (auto& cb : _observers) cb(_configData);
}

void DeviceManager::subscribe(std::function<void(const ConfigMap&)> cb) {
    _observers.push_back(std::move(cb));
}