#pragma once
#include <map>
#include <string>
#include <filesystem>
#include <sstream>
#include "CHTTPMethods.h"

class CPost : public CHTTPMethods{
public:
    std::stringstream& incoming( std::map< std::string, std::string >& headers, std::filesystem::path path,
                                 std::stringstream& message,  std::string& content, int cliSocket ) override;

private:
    static bool assemblePackets( size_t givenSize, int cliSocket, std::string& content );

    static std::stringstream& checkHeaders( std::map< std::string, std::string >& headers, std::stringstream& message );
};

