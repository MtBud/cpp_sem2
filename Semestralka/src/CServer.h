#pragma once
#include <map>
#include <string>
#include <vector>
#include "CHTTPMethods.h"

class CServer{
public:
    CServer();

    int start();

    void serve();

    void shutdown() const;



    static void reply( int cliSocket, std::stringstream& message );

private:
    int cliSocket;
    int srvrSocket;
    static std::vector<std::string> parse( std::string data, const std::string& delimiter );
    [[nodiscard]] bool requestSyntax( const std::vector< std::string >& requestLine,
                        const std::map< std::string, std::string >& headers,
                        const std::map< std::string, CHTTPMethods* >& methods) const;
};