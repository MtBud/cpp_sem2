#include <algorithm>
#include <iostream>
#include <ostream>
#include <fstream>
#include <sys/socket.h>
#include <filesystem>
#include "CConfig.h"
#include "CHTTPMethods.h"
#include "CUtils.h"
#include "CLogger.h"

#define BUFFER_SIZE 10240


std::stringstream& CGet::incoming( std::map< std::string, std::string >& headers, const std::filesystem::path& localPath, std::stringstream& message ){
    CConfig conf;
    CLogger logger;
    for( auto& i : conf.data["restricted"]){
        if( localPath.native().find(i) == 0){
            // check if correct password has been provided
            if( headers["Authorization"] != std::string("Basic ").append(conf.data["authentication"]["password"]) ){
                message << "HTTP/1.1 " << 401 << " Unauthorized" << "\r\n";
                message << "Content-Length: " << 0 << "\r\n";
                message << "Connection: " << "keep-alive" << "\r\n";
                return message;
            }
        }
    }

    std::string rootDir = conf.data["root"];
    std::filesystem::path path = rootDir;
    path += localPath;

    // 404 path doesn't exist
    if( ! std::filesystem::exists(path) ){
        message << "HTTP/1.1 " << 404 << " Not Found" << "\r\n";
        message << "Connection: " << "keep-alive" << "\r\n";
        message << "Content-Length: " << 0 << "\r\n";
        logger.log( "Unexisting path: " + path.native() );
        return message;
    }

    if( std::filesystem::is_directory(path) ){
        message << "HTTP/1.1 " << 200 << " OK" << "\r\n";
        message << "Connection: " << "keep-alive" << "\r\n";
        message << "Content-Type: text/plain; charset=UTF-8" << "\r\n";
        std::stringstream tmp;
        CContent::list( "", path, tmp );
        size_t length = tmp.str().length();
        message << "Content-Length: " << length << "\r\n";
        message << "\r\n";

        message << tmp.str();
        return message;
    }

    std::string extension( path.extension() );
    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower );
    if( extension == ".jpg" )
        extension = ".jpeg";
    if( extension == ".jpeg" || extension == ".png" ){
        message << "HTTP/1.1 " << 200 << " OK" << "\r\n";
        message << "Connection: " << "keep-alive" << "\r\n";
        message << "Content-Type: image/" << extension.substr(1) << "; charset=UTF-8" << "\r\n";
        std::ifstream ifs(path);
        std::string content( (std::istreambuf_iterator<char>(ifs) ),
                             (std::istreambuf_iterator<char>()    ) );
        message << "Content-Length: " << content.length() << "\r\n";
        message << "\r\n";

        message << content;
        return message;
    }

    if( extension == ".html" || extension == ".txt" ){
        message << "HTTP/1.1 " << 200 << " OK" << "\r\n";
        message << "Connection: " << "keep-alive" << "\r\n";
        if( extension == ".html" )
            message << "Content-Type: text/" << extension.substr(1) << "; charset=UTF-8" << "\r\n";
        else
            message << "Content-Type: text/plain; charset=UTF-8" << "\r\n";
        std::ifstream ifs(path);
        std::string content( (std::istreambuf_iterator<char>(ifs) ),
                             (std::istreambuf_iterator<char>()    ) );
        message << "Content-Length: " << content.length() << "\r\n";
        message << "\r\n";

        message << content;
        return message;
    }

    message << "HTTP/1.1 " << 415 << " Unsupported Media Type" << "\r\n";
    message << "Connection: " << "keep-alive" << "\r\n";
    message << "Content-Length: " << 0 << "\r\n";
    return message;
}

std::stringstream& CPost::incoming( std::map< std::string, std::string >& headers, const std::filesystem::path& path, std::stringstream& message ){
    return message;
}

std::stringstream& CPut::incoming( std::map< std::string, std::string >& headers, const std::filesystem::path& path, std::stringstream& message ){
    return message;
}

std::stringstream& CDelete::incoming( std::map< std::string, std::string >& headers, const std::filesystem::path& path, std::stringstream& message ){
    return message;
}

void CHTTPMethods::authenticate(){

}


void CHTTPMethods::badRequest( int cliSocket ){
    std::stringstream message;
    message << "HTTP/1.1 " << 400 << " Bad Request" << "\r\n";
    message << "Content-Length: " << 0 << "\r\n";
    message << "Connection: " << "keep-alive" << "\r\n";
    message << "\r\n";
    size_t length = message.str().length();
    send( cliSocket, message.str().c_str(), length, 0);
};