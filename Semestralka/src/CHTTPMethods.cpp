#include <iostream>
#include <ostream>
#include <sys/socket.h>
#include <filesystem>
#include "CConfig.h"
#include "CHTTPMethods.h"
#include "CUtils.h"

#define BUFFER_SIZE 10240


std::stringstream& CGet::incoming( std::map< std::string, std::string >& headers, const std::string& path, std::stringstream& message ){
    CConfig conf;
    for( auto& i : conf.data["restricted"]){
        if( path.find(i) == 0){
            // check if correct password has been provided
            if( headers["Authorization"] != std::string("Basic ").append(conf.data["authentication"]["password"]) ){
                message << "HTTP/1.1 " << 401 << " Unauthorized" << std::endl;
                message << "Connection: " << "keep-alive" << std::endl;
                return message;
            }
        }
    }

    // 404 path doesn't exist
    if( ! std::filesystem::exists(path) ){
        message << "HTTP/1.1 " << 404 << " Not Found" << std::endl;
        message << "Connection: " << "keep-alive" << std::endl;
        return message;
    }

    if( std::filesystem::is_directory(path) ){
        message << "HTTP/1.1 " << 200 << " OK" << std::endl;
        message << "Connection: " << "keep-alive" << std::endl;
        message << "Content-Type: text; charset=UTF-8" << std::endl;
        std::stringstream tmp;
        CContent::list( std::string( conf.data["root"] ), path, tmp );
        size_t length = tmp.str().length();
        message << "Content-Length: " << length << std::endl;
        message << std::endl;

        message << tmp.str();
        return message;
    }
}

std::stringstream& CPost::incoming( std::map< std::string, std::string >& headers, const std::string& path, std::stringstream& message ){

}

std::stringstream& CPut::incoming( std::map< std::string, std::string >& headers, const std::string& path, std::stringstream& message ){

}

std::stringstream& CDelete::incoming( std::map< std::string, std::string >& headers, const std::string& path, std::stringstream& message ){

}

void CHTTPMethods::authenticate(){

}


// message number -- 200 ok, 403, 404
// connection -- close, keep alive
std::stringstream& CHTTPMethods::reply( int cliSocket, const std::string& status, const std::string& connection, const std::string& path = "" ){
    CConfig conf;
    std::stringstream message;
    message << "HTTP/1.1 " << status << std::endl;
    message << "Accept-Ranges: bytes" << std::endl;

    if( std::filesystem::is_directory(path) ){
        message << "Content-Type: text/html; charset=UTF-8" << std::endl;
        std::stringstream tmp;
        CContent::list( conf.data["root"], path, tmp );
        std::string dirContent = tmp.str();
        message << "Content-Length: " << dirContent.size() << std::endl;
    }

    message << "Connection: " << connection << std::endl;

    std::string tmp = message.str();
    const char* buffer = tmp.c_str();
    if(send(cliSocket, buffer, strlen(buffer), MSG_NOSIGNAL) < 0){
        throw std::runtime_error("Unable to send data to client");
    }
}