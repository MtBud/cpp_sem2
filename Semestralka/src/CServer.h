#pragma once
#include <string>
#include <vector>

class CServer{
public:
    static int start();

    static void serve( int srvrSocket );

    static void console();

    static void shutdown( int srvrSocket, int cliSocket );

private:
    static std::vector<std::string> parseConsole();

    static std::vector<std::string> parseHTTP( const char* bytes );
};