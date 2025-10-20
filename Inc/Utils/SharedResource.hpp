#ifndef SHARED_RESOURCE_HPP
#define SHARED_RESOURCE_HPP

#include <mutex>
#include <future>

template <typename T>
class SharedResource {
public:
    void setData(T data) {
        std::lock_guard<std::mutex> lock(resourceMutex);
        sharedData = data;
    }

    T getData() {
        std::lock_guard<std::mutex> lock(resourceMutex);
        return sharedData;
    }

    std::future<T> getDataAsync() {
        return std::async(std::launch::async, &SharedResource::getData, this);
    }

private:
    T sharedData;
    std::mutex resourceMutex;
};

#endif // SHARED_RESOURCE_HPP