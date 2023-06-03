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
    static bool assemblePackets( size_t givenSize, int cliSocket, std::string& content );
    static std::filesystem::path mapAddress( const std::filesystem::path& path );
};


class CPost : public CHTTPMethods{
public:
    std::stringstream& incoming( std::map< std::string, std::string >& headers, std::filesystem::path path,
                                 std::stringstream& message,  std::string& content, int cliSocket ) override;
};

