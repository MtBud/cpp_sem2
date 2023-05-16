#pragma once
#include <string>
#include <vector>

class CHTTPMethods{
public:
    virtual void incoming( std::vector< std::string >& request, const std::string& path, int cliSocket ) = 0;
private:
    void authenticate();
    void reply( int cliSocket, int status, const std::string& connection a);
};

class CGet : public CHTTPMethods{
public:
    void incoming( std::vector< std::string >& request, const std::string& path, int cliSocket ) override;
};

class CPost : public CHTTPMethods{
public:
    void incoming( std::vector< std::string >& request, const std::string& path, int cliSocket ) override;
};

class CPut : public CHTTPMethods{
public:
    void incoming( std::vector< std::string >& request, const std::string& path, int cliSocket ) override;
};

class CDelete : public CHTTPMethods{
public:
    void incoming( std::vector< std::string >& request, const std::string& path, int cliSocket ) override;
};

