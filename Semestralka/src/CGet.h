#pragma once
#include <map>
#include <string>
#include <filesystem>
#include <sstream>
#include "CHTTPMethods.h"

class CGet : public CHTTPMethods{
public:
    std::stringstream& incoming( std::map< std::string, std::string >& headers, std::filesystem::path path,
                                 std::stringstream& message, std::string& content, int cliSocket ) override;
private:
    static void shutdown( int cliSocket );

    static std::stringstream& script( const std::filesystem::path& path, std::stringstream& message );
};