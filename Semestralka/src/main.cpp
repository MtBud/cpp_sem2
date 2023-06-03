#include <iostream>
#include <fstream>
#include <string>
#include "CServer.h"
#include "CLogger.h"
#include "CConfig.h"

int main(){
    CServer server;
    CConfig conf;
    conf.checkCorrectness();
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