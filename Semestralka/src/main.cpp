#include <iostream>
#include <string>
#include "CServer.h"
#include "CLogger.h"
int main(){
    CServer server;
    server.start();
    try{
        server.serve();
    }
    catch( std::string& ){
        CLogger::log("Server shutdown");
        server.shutdown();
    }
    return 1;
}