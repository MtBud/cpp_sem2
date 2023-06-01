#pragma once
#include <map>
#include <string>
#include <vector>
#include "CHTTPMethods.h"

class CServer{
public:
    static int start();

    static void serve( int srvrSocket );

    static void console();

    static void shutdown( int srvrSocket, int cliSocket = 0 );

    static bool requestSyntax( const std::vector< std::string >& requestLine,
                               const std::map< std::string, std::string >& headers,
                               const std::map< std::string, CHTTPMethods* >& methods,
                               int cliSocket);

    static void reply( int cliSocket, std::stringstream& message );

private:
    static std::vector<std::string> parse( std::string data, const std::string& delimiter );
};