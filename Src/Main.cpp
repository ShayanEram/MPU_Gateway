#include "Main.hpp"
#include "ThreadManager.hpp"

int main() 
{    
    ThreadManager::getInstance().startAll();

    std::this_thread::sleep_for(std::chrono::seconds(10)); // Let threads run

    ThreadManager::getInstance().stopAll();
    return 0;
}