#pragma once
#include <map>
#include <string>
#include <filesystem>
#include <sstream>
#include "CHTTPMethods.h"
#include "CConfig.h"

class CGet : public CHTTPMethods{
    std::map< std::string, std::string > headers;
    CConfig conf;

public:
    std::stringstream& incoming( const std::map< std::string, std::string >& headerMap, std::filesystem::path path,
                                 std::stringstream& message, std::string& content, int cliSocket ) override;

private:
    static void shutdown( int cliSocket );

    static std::stringstream& script( const std::filesystem::path& path, std::stringstream& message );
};