#ifndef OBSERVER_HPP
#define OBSERVER_HPP

#include <vector>
#include <string>
#include <functional>
#include <future>

template <typename T>
class Observer {
public:
    void addListener(std::function<void(T)> callback) {
        listeners.push_back(callback);
    }

    void triggerEvent(T eventMessage) {
        for (auto& listener : listeners) {
            listener(eventMessage);
        }
    }

    void triggerEventAsync(T eventMessage) {
        for (auto& listener : listeners) {
            std::async(std::launch::async, listener, eventMessage);
        }
    }

private:
    std::vector<std::function<void(T)>> listeners;
};

#endif // OBSERVER_HPP
