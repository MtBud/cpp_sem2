#include <iostream>
#include <ostream>
#include <sys/socket.h>
#include <filesystem>
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
void CHTTPMethods::reply( int cliSocket, const std::string& status, const std::string& connection, const std::string& path = "" ){
    CConfig conf;
    std::stringstream message;
    message << "HTTP/1.1 " << status << std::endl;

    if( std::filesystem::is_directory(path) ){
        message << "Content-Type: text/html; charset=UTF-8" << std::endl;
        std::stringstream tmp;
        CContent::list( conf.data["root"], path, tmp );
        std::string dirContent = tmp.str();
        message << "Content-Length: " << dirContent.size() << std::endl;
    }

    message << "Accept-Ranges: bytes" << std::endl;
    message << "Connection: " << connection << std::endl;


    std::string tmp = message.str();
    const char* buffer = tmp.c_str();
    if(send(cliSocket, buffer, strlen(buffer), MSG_NOSIGNAL) < 0){
        throw std::runtime_error("Unable to send data to client");
    }
}