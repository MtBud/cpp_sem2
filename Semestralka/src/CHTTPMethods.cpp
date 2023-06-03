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

#include "CConfig.h"
#include "CHTTPMethods.h"
#include "CUtils.h"
#include "CLogger.h"

#define BUFFER_SIZE 8196

CHTTPMethods::~CHTTPMethods() = default;

//-------------------------------------------------------------------------------------------------------------------

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


    if( ! is_directory( std::filesystem::path(conf.data["root"]) += localPath ) ){
        badRequest( "404 Not Found", message );
        CLogger::log( "Unexisting path: " + localPath.native() );
        return message;
    }

    if( headers.find("Content-Length") == headers.end() ){
        CLogger::log("Missing content length");
        return badRequest("411 Length Required", message);
    }
    if( stoi( headers[ "Content-Length" ] ) < 0 ){
        CLogger::log("Missing content length");
        return badRequest("411 Length Required", message);
    }
    unsigned int contentLength = (unsigned int) stoi( headers[ "Content-Length" ]);
    if( headers.find("Content-Type") == headers.end() ){
        CLogger::log("Missing content type");
        return badRequest("400 Bad Request", message);
    }

    if( contentLength != content.size() ){
        std::future< bool > f1 = std::async( std::launch::async, []( size_t contentLength, int cliSocket, std::string& content) {
            return assemblePackets( contentLength, cliSocket, content );
        }, contentLength, cliSocket, std::ref(content));
        if( f1.wait_for(std::chrono::seconds(1)) == std::future_status::timeout ){
            CLogger::log("Connection timed out");
            return badRequest("408 Request Timeout", message);
        }
    }


    std::string fileExt = headers["Content-Type"];
    if( fileExt.find('/') == std::string::npos ){
        CLogger::log("Bad content type format");
        return badRequest("400 Bad Request", message);
    }
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

    message << "HTTP/1.1 " << "200 OK" << "\r\n";
    message << "Content-Length: 0" << "\r\n";
    message << "Connection: " << "keep-alive" << "\r\n";
    message << "\r\n";

    return message;
}


std::stringstream& CHTTPMethods::badRequest( const std::string& response, std::stringstream& message ){
    message << "HTTP/1.1 " << response << "\r\n";
    message << "Content-Length: 0" << "\r\n";
    message << "Connection: " << "keep-alive" << "\r\n";
    message << "\r\n";
    return message;
}

std::filesystem::path CHTTPMethods::mapAddress( const std::filesystem::path& path ){
    CConfig conf;
    std::cout << "old address: " << path << std::endl;
    for( auto& i : conf.data["address-mapping"] ){
        if( path.native().find( i[0] ) == 0 ){
            std::string newPath = path.native();
            newPath.replace( 0, std::string( i[0] ).length(), std::string( i[1]) );
            std::cout << "mapped address: " << newPath << std::endl;
            return {newPath};
        }
    }
    return path;
}


bool CHTTPMethods::assemblePackets( size_t givenSize, int cliSocket, std::string& content ){
    char buffer[BUFFER_SIZE];
    ssize_t bytesRead;
    while( content.size() < givenSize ){
        bytesRead = recv(cliSocket, buffer, BUFFER_SIZE, 0);
        content += std::string(buffer, bytesRead);
    }
    return true;
}
