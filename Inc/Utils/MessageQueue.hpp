#ifndef MESSAGE_QUEUE_HPP
#define MESSAGE_QUEUE_HPP

#include <queue>
#include <mutex>
#include <condition_variable>
#include <future>

template <typename T>
class MessageQueue {
public:
    void pushCommand(const T& command) {
        std::lock_guard<std::mutex> lock(queueMutex);
        commands.push(command);
        condition.notify_one();
    }

    T popCommand() {
        std::unique_lock<std::mutex> lock(queueMutex);
        condition.wait(lock, [this]{ return !commands.empty(); });
        T command = commands.front();
        commands.pop();
        return command;
    }

    std::future<T> popCommandAsync() {
        return std::async(std::launch::async, &MessageQueue::popCommand, this);
    }

private:
    std::queue<T> commands;
    std::mutex queueMutex;
    std::condition_variable condition;
};

#endif // MESSAGE_QUEUE_HPP
