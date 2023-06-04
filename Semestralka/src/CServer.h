#pragma once
#include <map>
#include <string>
#include <vector>
#include "CHTTPMethods.h"

class CServer{
    int cliSocket;
    int srvrSocket;
    CHTTPMethods* get;
    CHTTPMethods* post;

public:
    CServer();

    int start();
    void serve();
    void shutdown() const;
    void reply( std::stringstream& message ) const;

private:
    [[nodiscard]] bool requestSyntax( const std::vector< std::string >& requestLine,
                                      const std::map< std::string, CHTTPMethods* >& methods) const;

    static std::map< std::string, std::string > parseHeaders( const std::vector< std::string >& request, bool& flag );

};