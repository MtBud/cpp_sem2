#include <algorithm>
#include <iostream>
#include <ostream>
#include <fstream>
#include <set>
#include <ctime>
#include <chrono>
#include <future>
#include <string>
#include <exception>
#include <thread>
#include <sys/socket.h>
#include <filesystem>
#include "CConfig.h"
#include "CHTTPMethods.h"
#include "CUtils.h"
#include "CLogger.h"
#include "CGetFormats.h"

#define BUFFER_SIZE 8196

std::stringstream& CGet::incoming( std::map< std::string, std::string >& headers, std::filesystem::path localPath,
                                   std::stringstream& message, std::string& data, int cliSocket ){
    CConfig conf;
    for( auto& i : conf.data["restricted"]){
        if( localPath.native().find(i) == 0){
            // check if correct password has been provided
            if( headers["Authorization"] != std::string("Basic ").append(conf.data["password"]) ){
                badRequest( "401 Unauthorized", message);
                return message;
            }
        }
    }

    if( localPath == std::string( conf.data["shutdown"] ) ){
        message << "HTTP/1.1 200 OK\r\n";
        message << "Content-Length: " << 0 << "\r\n";
        message << "Connection: " << "close" << "\r\n";
        message << "\r\n";
        throw std::runtime_error("Server shutdown");
    }

    std::string rootDir = conf.data["root"];
    std::filesystem::path path = rootDir;
    path += localPath;

    // 404 path doesn't exist
    if( ! std::filesystem::exists(path) ){
        badRequest( "404 Not Found", message );
        CLogger::log( "Unexisting path: " + path.native() );
        return message;
    }


    // write 200 OK at the beginning and choose which content type header to use
    std::string content;
    message << "HTTP/1.1 " << 200 << " OK" << "\r\n";
    if(headers["Connection"] == "close")
        message << "Connection: " << "close" << "\r\n";
    else
        message << "Connection: " << "keep-alive" << "\r\n";

    if( std::filesystem::is_directory(path) ){
        message << "Content-Type: text/plain; charset=UTF-8" << "\r\n";
        std::stringstream tmp;
        CContent::list( "", path, tmp );
        content = tmp.str();
    }

    std::string extension( path.extension() );
    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower );
    if( extension == ".jpg" )
        extension = ".jpeg";
    if( extension == ".jpeg" || extension == ".png" ){
        message << "Content-Type: image/" << extension.substr(1) << "\r\n";
        std::ifstream ifs(path, std::ios::binary );
        content = std::string( (std::istreambuf_iterator<char>(ifs) ),
                               (std::istreambuf_iterator<char>() ) );
    }

    if( extension == ".html" || extension == ".txt" ){
        if( extension == ".html" )
            message << "Content-Type: text/" << extension.substr(1) << "; charset=UTF-8" << "\r\n";
        else
            message << "Content-Type: text/plain; charset=UTF-8" << "\r\n";
        std::ifstream ifs(path);
        content = std::string( (std::istreambuf_iterator<char>(ifs) ),
                               (std::istreambuf_iterator<char>() ) );
    }


    // if none of the options were executed, flush the message stream and reply with error instead
    if( content.empty() ){
        message.str("");
        badRequest( "415 Unsupported Media Type", message);
    }
    else{
        message << "Content-Length: " << content.length() << "\r\n";
        message << "\r\n";
        message << content;
    }

    return message;
}

//-------------------------------------------------------------------------------------------------------------------

std::stringstream& CPost::incoming( std::map< std::string, std::string >& headers, std::filesystem::path localPath,
                                    std::stringstream& message, std::string& content, int cliSocket ){
    CConfig conf;
    if( localPath == "/" ){
        localPath = std::string( conf.data["post"] );
    }

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
    message << "Content-Length: " << 0 << "\r\n";
    message << "Connection: " << "keep-alive" << "\r\n";
    message << "\r\n";

    return message;
}


std::stringstream& CHTTPMethods::badRequest( const std::string& response, std::stringstream& message ){
    message << "HTTP/1.1 " << response << "\r\n";
    message << "Content-Length: " << 0 << "\r\n";
    message << "Connection: " << "keep-alive" << "\r\n";
    message << "\r\n";
    return message;
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
