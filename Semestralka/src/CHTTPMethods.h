#pragma once
#include <string>
#include <vector>
#include <filesystem>

class CHTTPMethods{
public:
    ~CHTTPMethods();

    virtual std::stringstream& incoming( std::map< std::string, std::string >& headers, std::filesystem::path path,
                                         std::stringstream& message,  std::string& content, int cliSocket ) = 0;

    static std::stringstream& badRequest( const std::string& response, std::stringstream& message );
    static std::filesystem::path mapAddress( const std::filesystem::path& path );
};


