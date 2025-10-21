#ifndef INC_UTILS_RINGBUFFER_HPP
#define INC_UTILS_RINGBUFFER_HPP

#include <vector>
#include <stdexcept>

template<typename T>
class RingBuffer {
public:
    explicit RingBuffer(std::size_t capacity): buf(capacity), head(0), tail(0), full(false) {}

    void push(const T& item) {
        buf[head] = item;
        if (full) tail = (tail + 1) % buf.size();
        head = (head + 1) % buf.size();
        full = head == tail;
    }

    T pop() {
        if (empty()) throw std::runtime_error("RingBuffer empty");
        auto val = buf[tail];
        full = false;
        tail = (tail + 1) % buf.size();
        return val;
    }

    bool empty() const { return (!full && head == tail); }
    bool isFull() const { return full; }
    std::size_t size() const {
        if (full) return buf.size();
        if (head >= tail) return head - tail;
        return buf.size() + head - tail;
    }

private:
    std::vector<T> buf;
    std::size_t head, tail;
    bool full;
};

#endif // INC_UTILS_RINGBUFFER_HPP