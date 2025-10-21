#include "CloudManager.hpp"

#include <future>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

CloudManager::CloudManager(const std::string& ip, int port): _serverIp(ip), _serverPort(port), _sockfd(-1) {
    // Constructor implementation
}

CloudManager::~CloudManager() { 
    disconnect(); 
}
//------------------------------------------------------------------------------------
bool CloudManager::connect() {
    disconnect();
    _sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (_sockfd < 0) {
        return false;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(_serverPort);
    if (::inet_pton(AF_INET, _serverIp.c_str(), &addr.sin_addr) <= 0) {
        return false;
    }

    if (::connect(_sockfd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
        ::close(_sockfd);
        _sockfd = -1;
        return false;
    }
    return true;
}

void CloudManager::disconnect() {
    if (_sockfd >= 0) { 
        ::close(_sockfd); _sockfd = -1; 
    }
}

void CloudManager::publish(const std::string& payload) {
    std::async(std::launch::async, [this, payload]() {
        if (_sockfd < 0 && !connect()) return;
        const char* buf = payload.c_str();
        std::size_t remaining = payload.size();
        while (remaining > 0) {
            ssize_t sent = ::send(_sockfd, buf, remaining, 0);
            if (sent <= 0) { connect(); return; }
            buf += sent;
            remaining -= static_cast<std::size_t>(sent);
        }
    });
}

std::string CloudManager::receiveOnce() {
    if (_sockfd < 0 && !connect()) return {};
    char buf[512];
    ssize_t n = ::recv(_sockfd, buf, sizeof(buf)-1, 0);
    if (n <= 0) return {};
    buf[n] = '\0';
    return std::string(buf);
}