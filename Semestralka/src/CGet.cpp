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
#include "CUtils.h"
#include "CLogger.h"
#include "CGetFormats.h"
#include "CGet.h"

std::stringstream& CGet::incoming( std::map< std::string, std::string >& headers, std::filesystem::path localPath,
                                   std::stringstream& message, std::string& data, int cliSocket ){
    CConfig conf;
    localPath = mapAddress( localPath );

    // check if folder is restricted
    for( auto& i : conf.data["restricted"]){
        if( localPath.native().find(i) == 0){
            // check if correct password has been provided
            if( headers["Authorization"] != std::string("Basic ").append(conf.data["password"]) ){
                return badRequest( "401 Unauthorized", message);
            }
        }
    }

    // check for shutdown
    if( localPath == std::string( conf.data["shutdown"] ) )
        CGet::shutdown( cliSocket );

    std::string rootDir = conf.data["root"];
    std::filesystem::path path = rootDir;
    path += localPath;

    // 404 path doesn't exist
    if( ! std::filesystem::exists(path) ){
        badRequest( "404 Not Found", message );
        CLogger::log( "Unexisting path: " + path.native() );
        return message;
    }

    // write 200 OK and shared headers at the beginning
    message << "HTTP/1.1 200 OK" << "\r\n";
    if(headers["Connection"] == "close")
        message << "Connection: " << "close" << "\r\n";
    else
        message << "Connection: " << "keep-alive" << "\r\n";

    if( std::filesystem::is_directory(path) ){
        CDir makeDir;
        return makeDir.compose( path, message );
    }


    // check if the file is a script
    std::string extension = path.extension();
    try{
        if( ! std::string( conf.data["script-execution"][extension] ).empty() )
            return script( path, message );
    }
    catch( nlohmann::json_abi_v3_11_2::detail::type_error& ){}


    // check if the file is of a supported format
    std::string type;
    try{
        type = conf.data["file-extensions"][extension];
    }
    catch( nlohmann::json_abi_v3_11_2::detail::type_error& ){
        message.str("");
        return badRequest("415 Unsupported Media Type", message);
    }


    CGetFormats* image = new( CImage );
    CGetFormats* text = new( CText );
    CGetFormats* video = new( CVideo );
    std::map <std::string, CGetFormats* > formats = {{"image", image },
                                                     {"text", text },
                                                     {"video", video}};
    formats[type]->compose( path, message );
    delete image;
    delete text;
    delete video;
    return message;
}

//----------------------------------------------------------------------------------------------------------------------

std::stringstream& CGet::script( const std::filesystem::path& filePath, std::stringstream& message ){
    CConfig conf;
    std::string filename = filePath.filename();
    std::string extension = filePath.extension();
    std::filesystem::path path = filePath.parent_path();
    std::filesystem::path startDir = std::filesystem::current_path();
    std::filesystem::current_path( path );
    std::string command;
    try{
        command = conf.data["script-execution"][extension];
    }
    catch( nlohmann::json_abi_v3_11_2::detail::type_error& ){
        message.str("");
        std::filesystem::current_path(startDir);
        return CGet::badRequest("415 Unsupported Media Type", message);
    }

    if( command.find("filename") == std::string::npos ){
        message.str("");
        std::filesystem::current_path(startDir);
        return CGet::badRequest("500 Internal Server Error", message);
    }

    while( command.find("filename") != std::string::npos )
        command.replace( command.find("filename"), std::string("filename").length(), filename );
    std::system( command.c_str() );
    message << "Content-Length: " << 0 << "\r\n";
    message << "\r\n";
    std::filesystem::current_path(startDir);
    return message;
}

//----------------------------------------------------------------------------------------------------------------------

void CGet::shutdown( int cliSocket ){
    std::stringstream message;
    message << "HTTP/1.1 200 OK\r\n";
    message << "Content-Length: " << 0 << "\r\n";
    message << "Connection: " << "close" << "\r\n";
    message << "\r\n";

    size_t length = message.str().length();
    CLogger::log("SENDING");
    CLogger::log(message.str().substr(0, message.str().find("\r\n\r\n") + 4));
    send( cliSocket, message.str().c_str(), length, 0);
    throw std::string("shutdown");
}