#pragma once
#include <map>
#include <string>
#include <filesystem>
#include <sstream>
#include "CHTTPMethods.h"
#include "CConfig.h"

class CPost : public CHTTPMethods{
    std::map< std::string, std::string > headers;
    CConfig conf;

public:
    std::stringstream& incoming( const std::map< std::string, std::string >& headersMap, std::filesystem::path path,
                                 std::stringstream& message,  std::string& content, int cliSocket ) override;

private:
    static bool assemblePackets( size_t givenSize, int cliSocket, std::string& content );

    std::stringstream& checkHeaders( std::stringstream& message );

    std::filesystem::path makeName( const std::filesystem::path& localPath );
};

