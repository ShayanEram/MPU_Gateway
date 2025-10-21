#ifndef INC_UTILS_MESSAGEQUEUE_HPP_
#define INC_UTILS_MESSAGEQUEUE_HPP_

#include "ThreadSafeQueue.hpp"

template<typename T>
class MessageQueue {
public:
    void send(const T& msg) { queue.push(msg); }
    void send(T&& msg) { queue.push(std::move(msg)); }

    T receive() 
    {
        T msg;
        queue.pop(msg);
        return msg;
    }

    bool try_receive(T& msg) { return queue.try_pop(msg); }

private:
    ThreadSafeQueue<T> queue;
};

#endif // INC_UTILS_MESSAGEQUEUE_HPP_