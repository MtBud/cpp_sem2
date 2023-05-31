#pragma once
#include <string>
#include <vector>
#include <filesystem>

class CHTTPMethods{
public:
    virtual std::stringstream& incoming( std::map< std::string, std::string >& headers, const std::filesystem::path& path, std::stringstream& message, const std::string& content ) = 0;
    static std::stringstream& badRequest( const std::string& response, std::stringstream& message );
};

class CGet : public CHTTPMethods{
public:
    std::stringstream& incoming( std::map< std::string, std::string >& headers, const std::filesystem::path& path, std::stringstream& message, const std::string& content ) override;
};

class CPost : public CHTTPMethods{
public:
    std::stringstream& incoming( std::map< std::string, std::string >& headers, const std::filesystem::path& path, std::stringstream& message, const std::string& content ) override;
};

