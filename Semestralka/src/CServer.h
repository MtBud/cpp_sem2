#pragma once
#include <map>
#include <string>
#include <vector>
#include "CHTTPMethods.h"

/**
 * @class CServer
 * @brief Functions as a basic server interface
 */
class CServer{
    /**
     * @brief Socket of the currently connected client
     */
    int cliSocket;
    /**
     * @brief Socket reserved by the server
     */
    int srvrSocket;
    CHTTPMethods* get;
    CHTTPMethods* post;

public:
    /**
     * @brief A constructor
     */
    CServer();

    /**
     * @brief Starts the server
     *
     * Makes the server listen on the port and interface defined in the config file @see CConfig.
     * Fills in the cliSocket and srvrSocket members.
     */
    int start();
    /**
     * @brief Handles client connections and transmissions
     *
     * Connects clients and handles back and forth messaging using @see CGet and @see CPost
     */
    void serve();
    /**
     * @brief Shuts down the server
     *
     * Shuts down the server and frees all allocated and reserved resources
     */
    void shutdown() const;

    /**
     * @brief Sends a message to the client
     * @param message Stream containing the message being sent
     */
    void reply( std::stringstream& message ) const;

private:
    /**
     * @brief Checks for correct HTTP formatting
     *
     * Checks for basic syntax errors that are not dependent on the HTTP method being used or file being sent/accepted
     * @param requestLine Parsed request line of HTTP message
     * @param methods     Map of the accepted HTTP methods
     * @return  Returns true if an issue has been found, returns false otherwise.
     */
    [[nodiscard]] bool requestSyntax( const std::vector< std::string >& requestLine,
                                      const std::map< std::string, CHTTPMethods* >& methods) const;

    /**
     * @brief Parses HTTP headers into a map
     *
     * The map being produced has header names as keys and the header values as its values
     * @param request   Header field of the HTTP request
     * @param flag      Flag is set to true if an error occured ( wrong header format )
     * @return Returns the map of headers
     */
    static std::map< std::string, std::string > parseHeaders( const std::vector< std::string >& request, bool& flag );

};