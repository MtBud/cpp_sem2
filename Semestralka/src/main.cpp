#include <thread>
#include <iostream>
#include "CServer.h"
int main(){
    // int srvrSocket = CServer::start();
    CServer::console();

    // std::thread consoleThread( &CServer::console);
    // CServer::serve(srvrSocket);
    std::cout << "bruh" << std::endl;
    return 1;
}