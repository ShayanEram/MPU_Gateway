#ifndef INC_OTAMANAGER_HPP
#define INC_OTAMANAGER_HPP

class OtaManager {
public:
    OtaManager() = default;
    ~OtaManager() = default;

    void start();
    void stop();
    void loop();
    bool checkForUpdates();
    bool downloadUpdate();
    bool validateDownload();
    void applyUpdate();

private:

};

#endif // INC_OTAMANAGER_HPP