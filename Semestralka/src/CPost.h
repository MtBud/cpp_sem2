#pragma once
#include <map>
#include <string>
#include <filesystem>
#include <sstream>
#include "CHTTPMethods.h"
#include "CConfig.h"

/**
 * @class CPost
 * @brief Used for making responses to POST requests
 */
class CPost : public CHTTPMethods{
    std::map< std::string, std::string > headers;
    CConfig conf;

public:
    /**
     * @see CHTTPMethods::incoming()
     */
    std::stringstream& incoming( const std::map< std::string, std::string >& headersMap, std::filesystem::path path,
                                 std::stringstream& message,  std::string& content, int cliSocket ) override;

private:
    /**
     * @brief Method used to assemble packets, in case the client request is too long
     *
     * @param givenSize Length taken from "Content-Length: " header
     * @param cliSocket Client socket
     * @param content   String that the content body appends to. Is not returned, but contains the content after
     *                  the function executes
     * @return Returns true if the packets were assembled without a problem.
     *         Doesn't return false.
     */
    static bool assemblePackets( size_t givenSize, int cliSocket, std::string& content );

    /**
     * @brief Check header formating for any errors
     *
     * @param message Stream used for outputting error messages
     * @return Returns true if an error occured, returns false if everything is in order
     */
    bool checkHeaders( std::stringstream& message );

    /**
     * @brief Method used for making new and unique file names
     *
     * @param localPath Path to the directory the file should be created in
     * @return Returns a unique filesystem path
     */
    std::filesystem::path makeName( const std::filesystem::path& localPath );
};

