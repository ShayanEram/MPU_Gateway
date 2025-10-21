#ifndef INC_CLOUDMANAGER_HPP_
#define INC_CLOUDMANAGER_HPP_

#include <string>
#include <mutex>

class CloudManager 
{
public:
    CloudManager(const std::string& ip, int port);
    ~CloudManager();

    bool connect();
    void disconnect();
    void publish(const std::string& payload);
    std::string receiveOnce();

private:
    std::string _serverIp;
    int _serverPort;
    int _sockfd;
    std::mutex _connMutex;
};

#endif // INC_CLOUDMANAGER_HPP_