#pragma once
#include <string>
#include <vector>
#include <filesystem>

class CHTTPMethods{
public:
    virtual std::stringstream& incoming( std::map< std::string, std::string >& headers, std::filesystem::path path,
                                         std::stringstream& message,  std::string& content, int cliSocket ) = 0;
    static std::stringstream& badRequest( const std::string& response, std::stringstream& message );
    static bool assemblePackets( size_t givenSize, int cliSocket, std::string& content );
};

class CGet : public CHTTPMethods{
public:
    std::stringstream& incoming( std::map< std::string, std::string >& headers, std::filesystem::path path,
                                 std::stringstream& message, std::string& content, int cliSocket ) override;
};

class CPost : public CHTTPMethods{
public:
    std::stringstream& incoming( std::map< std::string, std::string >& headers, std::filesystem::path path,
                                 std::stringstream& message,  std::string& content, int cliSocket ) override;
};

