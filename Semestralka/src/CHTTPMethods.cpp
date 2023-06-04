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
#include "util.h"
#include "CLogger.h"

CHTTPMethods::~CHTTPMethods() = default;

std::stringstream& CHTTPMethods::badRequest( const std::string& response, std::stringstream& message ){
    message << "HTTP/1.1 " << response << "\r\n";
    message << "Content-Length: 0" << "\r\n";
    message << "Connection: " << "keep-alive" << "\r\n";
    message << "\r\n";
    return message;
}

std::filesystem::path CHTTPMethods::mapAddress( const std::filesystem::path& path ){
    CConfig conf;
    for( auto& i : conf.data["address-mapping"] ){
        if( path.native().find( i[0] ) == 0 ){
            std::string newPath = path.native();
            newPath.replace( 0, std::string( i[0] ).length(), std::string( i[1]) );
            return {newPath};
        }
    }
    return path;
}