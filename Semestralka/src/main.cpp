#include <iostream>
#include "CServer.h"
int main(){
    int srvrSocket = CServer::start();
    CServer::serve(srvrSocket);
    std::cout << "PROGRAM TERMINATED" << std::endl;
    return 1;
}