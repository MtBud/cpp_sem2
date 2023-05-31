#include <algorithm>
#include <iostream>
#include <ostream>
#include <fstream>
#include <set>
#include <ctime>
#include <string>
#include <sys/socket.h>
#include <filesystem>
#include "CConfig.h"
#include "CHTTPMethods.h"
#include "CUtils.h"
#include "CLogger.h"

#define BUFFER_SIZE 8196


std::stringstream& CGet::incoming( std::map< std::string, std::string >& headers, const std::filesystem::path& localPath, std::stringstream& message, const std::string& data ){
    CConfig conf;
    for( auto& i : conf.data["restricted"]){
        if( localPath.native().find(i) == 0){
            // check if correct password has been provided
            if( headers["Authorization"] != std::string("Basic ").append(conf.data["authentication"]["password"]) ){
                badRequest( "401 Unauthorized", message);
                return message;
            }
        }
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

std::stringstream& CPost::incoming( std::map< std::string, std::string >& headers, const std::filesystem::path& localPath, std::stringstream& message, const std::string& content ){
    CConfig conf;
    if( ! is_directory( std::filesystem::path(conf.data["root"]) += localPath ) ){
        badRequest( "404 Not Found", message );
        CLogger::log( "Unexisting path: " + localPath.native() );
        return message;
    }

    // check if the path is available to normal users
    unsigned int i;
    for( i = 0; i < conf.data["post"].size(); i++){
        std::filesystem::path basePath = conf.data["post"][i];
        if(basePath == localPath)
            break;
        auto rel = std::filesystem::relative(localPath, basePath);
        if( rel.native()[0] == '.' )
            break;
    }
    // if not, check for authorization
    if( i == conf.data["post"].size() ){
        if( headers["Authorization"] != std::string("Basic ").append(conf.data["authentication"]["password"]) ){
            return badRequest( "401 Unauthorized", message);
        }
    }

    if( headers.find("Content-length") == headers.end() ){
        return badRequest("411 Length Required", message);
    }

    std::string fileExt = headers["Content-Type"];
    fileExt = fileExt.substr(fileExt.find('/'));
    std::stringstream newName;
    newName << std::time_t();
    std::ofstream outFile(localPath.native() + newName.str() + '.' + fileExt, std::ios::binary);
    if( ! outFile ){
        CLogger::log( "File couldn't be created" );
        return badRequest( "500 Internal Server Error", message);
    }

    outFile.write(content.c_str(), stoi(headers["Content-Length"]) );
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