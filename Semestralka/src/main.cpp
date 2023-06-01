#include <iostream>
#include <string>
#include "CServer.h"
#include "CLogger.h"
int main(){
    int srvrSocket = CServer::start();
    try{
        CServer::serve(srvrSocket);
    }
    catch( std::string& exception){
        CLogger::log("Server shutdown");
        CServer::shutdown( srvrSocket );
    }
    return 1;
}