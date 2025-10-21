#ifndef INC_UTILS_RAIIHANDLE_HPP
#define INC_UTILS_RAIIHANDLE_HPP

#include <unistd.h>

class RAIIHandle {
public:
    explicit RAIIHandle(int fd = -1) : fd(fd) {}
    ~RAIIHandle() { reset(); }

    RAIIHandle(const RAIIHandle&) = delete;
    RAIIHandle& operator=(const RAIIHandle&) = delete;

    RAIIHandle(RAIIHandle&& other) noexcept : fd(other.fd) {
        other.fd = -1;
    }
    RAIIHandle& operator=(RAIIHandle&& other) noexcept {
        if (this != &other) {
            reset();
            fd = other.fd;
            other.fd = -1;
        }
        return *this;
    }

    int get() const { return fd; }
    explicit operator bool() const { return fd >= 0; }

    void reset(int newFd = -1) {
        if (fd >= 0) ::close(fd);
        fd = newFd;
    }

private:
    int fd;
};

#endif // INC_UTILS_RAIIHANDLE_HPP