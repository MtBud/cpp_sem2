#include <iostream>
#include <sys/socket.h>
#include "CConfig.h"
#include "CHTTPMethods.h"
#include "CUtils.h"

#define BUFFER_SIZE 10240


void CGet::incoming( std::vector< std::string >& request, const std::string& path, int cliSocket ){
    CConfig conf;
    for( auto& i : conf.data["restricted"]){
        if( path.find(i) == 0){
            // check for password
            // send access denied
        }
    }
}

void CPost::incoming( std::vector< std::string >& request, const std::string& path, int cliSocket ){

}

void CPut::incoming( std::vector< std::string >& request, const std::string& path, int cliSocket ){

}

void CDelete::incoming( std::vector< std::string >& request, const std::string& path, int cliSocket ){

}

void CHTTPMethods::authenticate(){

}


// message number -- 200 ok, 403, 404
// connection -- close, keep alive
void CHTTPMethods::reply( int cliSocket, int status, const std::string& connection ){
    std::string message;
    message.append("HTTP/1.1 ");
    message.append(std::to_string(status));


    const char* buffer = message.c_str();
    if(send(cliSocket, buffer, strlen(buffer), MSG_NOSIGNAL) < 0){
        throw std::runtime_error("Unable to send data to client");
    }
}