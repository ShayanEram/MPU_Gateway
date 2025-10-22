#include <gtest/gtest.h>
#include "ThreadSafeQueue.hpp"
#include "SensorManager.hpp"  // for SensorData, SensorType

TEST(ThreadSafeQueueTest, PushAndPopWorks) {
    ThreadSafeQueue<SensorData> queue;

    // Create a dummy sensor data
    SensorData input{std::chrono::system_clock::now(), 42.0, SensorType::UART};

    // Push into queue
    queue.push(input);

    // Pop from queue
    SensorData output;
    bool ok = queue.try_pop(output);

    EXPECT_TRUE(ok);
    EXPECT_EQ(output.value, 42.0);
    EXPECT_EQ(output.type, SensorType::UART);
}
