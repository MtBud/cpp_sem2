#pragma once
#include <map>
#include <string>
#include <filesystem>
#include <sstream>
#include "CHTTPMethods.h"
#include "CConfig.h"

/**
 * @class CGet
 * @brief Used for making responses to GET requests
 */
class CGet : public CHTTPMethods{
    std::map< std::string, std::string > headers;
    CConfig conf;

public:
    /**
     * @see CHTTPMethods::incoming()
     */
    std::stringstream& incoming( const std::map< std::string, std::string >& headerMap, std::filesystem::path path,
                                 std::stringstream& message, std::string& content, int cliSocket ) override;

private:
    /**
     * @brief Method used for remote server shutdown
     *
     * Method creates and sends an 200 OK response. Then it throws an exception for @see main() in order to execute
     * server shutdown. @see CServer::shutdown()
     *
     * @param cliSocket Client socket
     */
    static void shutdown( int cliSocket );

    /**
     * @brief Method used for handling script execution requests
     *
     * @param path Filesystem path to the script
     * @param message Stream used for making server response
     * @return Returns "message" stream
     */
    static std::stringstream& script( const std::filesystem::path& path, std::stringstream& message );
};