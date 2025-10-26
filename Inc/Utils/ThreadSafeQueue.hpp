#ifndef INC_UTILS_THREADSAFEQUEUE_HPP_INCLUDED
#define INC_UTILS_THREADSAFEQUEUE_HPP_INCLUDED

#include <queue>
#include <mutex>
#include <condition_variable>

template<typename T>
class ThreadSafeQueue {
public:
    ThreadSafeQueue() = default;
    
    ThreadSafeQueue(const ThreadSafeQueue&) = delete;
    ThreadSafeQueue& operator=(const ThreadSafeQueue&) = delete;

    void push(const T& value) 
    {
        {
            std::lock_guard<std::mutex> lock(mtx);
            q.push(value);
        }
        cv.notify_one();
    }

    void push(T&& value) 
    {
        {
            std::lock_guard<std::mutex> lock(mtx);
            q.push(std::move(value));
        }
        cv.notify_one();
    }

    bool pop(T& out) 
    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this]{ return !q.empty(); });
        out = std::move(q.front());
        q.pop();
        return true;
    }

    bool try_pop(T& out) {
        std::lock_guard<std::mutex> lock(mtx);
        if (q.empty()) return false;
        out = std::move(q.front());
        q.pop();
        return true;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(mtx);
        return q.empty();
    }

private:
    mutable std::mutex mtx;
    std::condition_variable cv;
    std::queue<T> q;
};

#endif // INC_UTILS_THREADSAFEQUEUE_HPP_INCLUDED