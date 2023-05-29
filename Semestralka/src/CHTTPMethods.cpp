#include <algorithm>
#include <iostream>
#include <ostream>
#include <fstream>
#include <sys/socket.h>
#include <filesystem>
#include "CConfig.h"
#include "CHTTPMethods.h"
#include "CUtils.h"

#define BUFFER_SIZE 10240


std::stringstream& CGet::incoming( std::map< std::string, std::string >& headers, const std::filesystem::path& path, std::stringstream& message ){
    CConfig conf;
    for( auto& i : conf.data["restricted"]){
        if( path.native().find(i) == 0){
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
        message << "Content-Type: text/plain; charset=UTF-8" << std::endl;
        std::stringstream tmp;
        CContent::list( std::string( conf.data["root"] ), path, tmp );
        size_t length = tmp.str().length();
        message << "Content-Length: " << length << std::endl;
        message << std::endl;

        message << tmp.str();
        return message;
    }

    std::string extension( path.extension() );
    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower );
    if( extension == ".jpg" )
        extension = ".jpeg";
    if( extension == ".jpeg" || extension == ".png" ){
        message << "HTTP/1.1 " << 200 << " OK" << std::endl;
        message << "Connection: " << "keep-alive" << std::endl;
        message << "Content-Type: image/" << extension.substr(1) << "; charset=UTF-8" << std::endl;
        std::ifstream ifs(path);
        std::string content( (std::istreambuf_iterator<char>(ifs) ),
                             (std::istreambuf_iterator<char>()    ) );
        message << "Content-Length: " << content.length() << std::endl;
        message << std::endl;

        message << content;
        return message;
    }

    if( extension == ".html" || extension == ".txt" ){
        message << "HTTP/1.1 " << 200 << " OK" << std::endl;
        message << "Connection: " << "keep-alive" << std::endl;
        if( extension == ".html" )
            message << "Content-Type: text/" << extension.substr(1) << "; charset=UTF-8" << std::endl;
        else
            message << "Content-Type: text/plain; charset=UTF-8" << std::endl;
        std::ifstream ifs(path);
        std::string content( (std::istreambuf_iterator<char>(ifs) ),
                             (std::istreambuf_iterator<char>()    ) );
        message << "Content-Length: " << content.length() << std::endl;
        message << std::endl;

        message << content;
        return message;
    }

    message << "HTTP/1.1 " << 415 << " Unsupported Media Type" << std::endl;
    message << "Connection: " << "keep-alive" << std::endl;
    return message;
}

std::stringstream& CPost::incoming( std::map< std::string, std::string >& headers, const std::filesystem::path& path, std::stringstream& message ){

}

std::stringstream& CPut::incoming( std::map< std::string, std::string >& headers, const std::filesystem::path& path, std::stringstream& message ){

}

std::stringstream& CDelete::incoming( std::map< std::string, std::string >& headers, const std::filesystem::path& path, std::stringstream& message ){

}

void CHTTPMethods::authenticate(){

}


// message number -- 200 ok, 403, 404
// connection -- close, keep alive
std::stringstream& CHTTPMethods::sendFile( const std::filesystem::path& path ){

}

void CHTTPMethods::badRequest( int cliSocket ){
    std::stringstream out;
    out << "HTTP/1.1 " << 400 << " Bad Request" << std::endl;
    out << "Connection: " << "keep-alive" << std::endl;
    size_t length = out.str().length();
    send( cliSocket, out.str().c_str(), length, 0);
};