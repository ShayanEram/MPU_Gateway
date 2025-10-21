#ifndef INC_OTAMANAGER_HPP
#define INC_OTAMANAGER_HPP

#include <string>
#include <future>
#include <atomic>
#include <chrono>

class OtaManager {
public:
    OtaManager(const std::string& metaUrl, const std::string& dlPath);
    ~OtaManager();

    void start();
    void stop();

private:
    void loop();
    bool checkForUpdates();
    bool downloadUpdate();
    bool validateDownload();
    void applyUpdate();

    std::string _metadataUrl;
    std::string _downloadPath;
    std::future<void> _worker;
    std::atomic<bool> _running;
    std::chrono::system_clock::time_point _lastApplied;
};



#endif // INC_OTAMANAGER_HPP