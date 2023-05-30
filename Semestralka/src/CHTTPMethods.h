#pragma once
#include <string>
#include <vector>
#include <filesystem>

class CHTTPMethods{
public:
    virtual std::stringstream& incoming( std::map< std::string, std::string >& headers, const std::filesystem::path& path, std::stringstream& message ) = 0;
    static void badRequest( int cliSocket );
private:
    void authenticate();
};

class CGet : public CHTTPMethods{
public:
    std::stringstream& incoming( std::map< std::string, std::string >& headers, const std::filesystem::path& path, std::stringstream& message ) override;
};

class CPost : public CHTTPMethods{
public:
    std::stringstream& incoming( std::map< std::string, std::string >& headers, const std::filesystem::path& path, std::stringstream& message ) override;
};

