#include <algorithm>
#include <iostream>
#include <ostream>
#include <fstream>
#include <set>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <future>
#include <string>
#include <exception>
#include <thread>
#include <climits>
#include <sys/socket.h>
#include <filesystem>

#include "CPost.h"
#include "CConfig.h"
#include "CUtils.h"
#include "CLogger.h"

#define BUFFER_SIZE 8196

std::stringstream& CPost::incoming( std::map< std::string, std::string >& headers, std::filesystem::path localPath,
                                    std::stringstream& message, std::string& content, int cliSocket ){
    CConfig conf;
    localPath = mapAddress( localPath );

    // check if the path is available to normal users
    std::filesystem::path basePath = conf.data["post"];
    auto rel = std::filesystem::relative(localPath, basePath);
    if(basePath != localPath && rel.native()[0] != '.'){
        if( headers["Authorization"] != std::string("Basic ").append(conf.data["authentication"]["password"]) ){
            return badRequest( "401 Unauthorized", message);
        }
    }

    // check if directory exists
    if( ! is_directory( std::filesystem::path(conf.data["root"]) += localPath ) ){
        badRequest( "404 Not Found", message );
        CLogger::log( "Unexisting path: " + localPath.native() );
        return message;
    }

    // check headers
    size_t messageLen = message.str().length();
    if( checkHeaders( headers, message ).str().length() != messageLen )
        return message;

    // assemble all available packets, timeout after one second
    unsigned int contentLength = (unsigned int) stoi( headers[ "Content-Length" ]);
    if( contentLength != content.size() ){
        std::future< bool > f1 = std::async( std::launch::async, []( size_t contentLength, int cliSocket, std::string& content){
            return assemblePackets( contentLength, cliSocket, content );
        }, contentLength, cliSocket, std::ref(content));

        if( f1.wait_for(std::chrono::seconds(1)) == std::future_status::timeout ){
            CLogger::log("Connection timed out");
            return badRequest("408 Request Timeout", message);
        }
    }

    // generate a new file name based on the time it was recieved
    std::string fileExt = headers["Content-Type"];
    fileExt = fileExt.substr(fileExt.find('/') + 1);
    std::filesystem::path newName;
    do{
        std::stringstream currTime;
        currTime << std::time( nullptr );
        newName = ( std::string( conf.data["root"] ) + localPath.native() + '/' + currTime.str() + '.' + fileExt );
    }while( std::filesystem::exists(newName) );

    std::ofstream outFile( newName, std::ios::binary );
    if( ! outFile ){
        CLogger::log( "File couldn't be created" );
        return badRequest( "500 Internal Server Error", message);
    }

    outFile << content;
    outFile.close();

    // send back an ok message
    message << "HTTP/1.1 " << "200 OK" << "\r\n";
    message << "Content-Length: 0" << "\r\n";
    message << "Connection: " << "keep-alive" << "\r\n";
    message << "\r\n";

    return message;
}

//----------------------------------------------------------------------------------------------------------------------

std::stringstream& CPost::checkHeaders( std::map< std::string, std::string >& headers, std::stringstream& message ){
    CConfig conf;
    if( headers.find("Content-Length") == headers.end() ){
        CLogger::log("Missing content length");
        return badRequest("411 Length Required", message);
    }

    if( stoi( headers[ "Content-Length" ] ) < 0 ){
        CLogger::log("Missing content length");
        return badRequest("411 Length Required", message);
    }

    if( headers.find("Content-Type") == headers.end() ){
        CLogger::log("Missing content type");
        return badRequest("400 Bad Request", message);
    }

    if( headers["Content-Type"].find('/') == std::string::npos ){
        CLogger::log("Bad content type format");
        return badRequest("400 Bad Request", message);
    }

    return message;
}

//----------------------------------------------------------------------------------------------------------------------

bool CPost::assemblePackets( size_t givenSize, int cliSocket, std::string& content ){
    char buffer[BUFFER_SIZE];
    ssize_t bytesRead;
    while( content.size() < givenSize ){
        bytesRead = recv(cliSocket, buffer, BUFFER_SIZE, 0);
        content += std::string(buffer, bytesRead);
    }
    return true;
}
