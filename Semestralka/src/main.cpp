#include <thread>
#include <iostream>
#include "CServer.h"
int main(){
    int srvrSocket = CServer::start();
    std::thread consoleThread( &CServer::console);
    CServer::serve(srvrSocket);
    std::cout << "PROGRAM TERMINATED" << std::endl;
    return 1;
}