#ifndef INC_CLOUDMANAGER_HPP_
#define INC_CLOUDMANAGER_HPP_

#include <string>

class CloudManager 
{
public:
    CloudManager() = default;
    ~CloudManager() = default;

    bool connect();
    void disconnect();
    void publish(const std::string& payload);
    std::string receiveOnce();

private:

};

#endif // INC_CLOUDMANAGER_HPP_