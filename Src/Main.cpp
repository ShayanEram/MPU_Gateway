#include "Main.hpp"
#include "GatewayController.hpp"

#include <iostream>

int main(int argc, char** argv) {
    std::string cfg = (argc > 1) ? argv[1] : "/etc/gateway.conf";
    
    try 
    {
        GatewayController controller(cfg);
        controller.start();
    } 
    catch (const std::exception& e) {
        std::cerr << "Gateway crashed: " << e.what() << "\n";
        return 1;
    }
    return 0;
}