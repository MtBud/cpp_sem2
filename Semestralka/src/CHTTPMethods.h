#pragma once
#include <string>
#include <vector>

class CHTTPMethods{
public:
    virtual std::stringstream& incoming( std::map< std::string, std::string >& headers, const std::filesystem::path& path, std::stringstream& message ) = 0;
    static void badRequest( int cliSocket );
private:
    void authenticate();
    std::stringstream& sendFile( const std::filesystem::path& path );
};

class CGet : public CHTTPMethods{
public:
    std::stringstream& incoming( std::map< std::string, std::string >& headers, const std::filesystem::path& path, std::stringstream& message ) override;
};

class CPost : public CHTTPMethods{
public:
    std::stringstream& incoming( std::map< std::string, std::string >& headers, const std::filesystem::path& path, std::stringstream& message ) override;
};

class CPut : public CHTTPMethods{
public:
    std::stringstream& incoming( std::map< std::string, std::string >& headers, const std::filesystem::path& path, std::stringstream& message ) override;
};

class CDelete : public CHTTPMethods{
public:
    std::stringstream& incoming( std::map< std::string, std::string >& headers, const std::filesystem::path& path, std::stringstream& message ) override;
};

