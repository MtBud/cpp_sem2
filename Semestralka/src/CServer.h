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
    void reply( std::stringstream& message ) const;

private:
    int cliSocket;
    int srvrSocket;
    static std::vector<std::string> parse( std::string data, const std::string& delimiter );
    [[nodiscard]] bool requestSyntax( const std::vector< std::string >& requestLine,
                                      const std::map< std::string, CHTTPMethods* >& methods) const;
};