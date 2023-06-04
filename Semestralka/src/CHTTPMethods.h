#pragma once
#include <string>
#include <vector>
#include <filesystem>

/**
 * @class CHTTPMethods
 * @brief Abstract class used to derive individual HTTP method classes.
 */
class CHTTPMethods{
public:
    /**
     * @brief A virtual constructor
     */
    virtual ~CHTTPMethods();

    /**
     * @brief Function used for making replies to individual messages.
     *
     * "incoming" is the main function of all derived classes. It serves as a way to check and read the
     * client message and assemble a propper response.
     *
     * @param headerMap map of headers from the client's message
     * @see CServer::parseHeaders()
     *
     * @param path Filesystem path from the HTTP request line
     *
     * @param message Stream used for assembling the message.
     *
     * @param content Content body of the HTTP message.
     *
     * @param cliSocket Client socket used for sending packets.
     *
     * @return Function returns the "message" stream. Depending on the method, it includes all the data needed for
     * the message to be sent: request line, headers, and also content body when needed.
     */
    virtual std::stringstream& incoming( const std::map< std::string, std::string >& headerMap, std::filesystem::path path,
                                         std::stringstream& message,  std::string& content, int cliSocket ) = 0;

    /**
     * @brief Method used for making short responses with no content body
     *
     * Although it is named badRequest, it can be used for any type of response. It produces a message in the
     * following format:
     *
     * HTTP/1.1 "response"\r\n
     * Content-Length: 0\r\n
     * Connection: keep-alive\r\n\r\n
     *
     * @param response String used in the request line.
     * @param message Stream in which the message will be written into.
     * @return Returns "message" stream
     */
    static std::stringstream& badRequest( const std::string& response, std::stringstream& message );

    /**
     * @brief Method used for substituting filesystem paths
     *
     * Method checks the configuration for "address-mapping" @see CConfig::checkCorectness(). It substitues
     * the first path with the second one.
     *
     * @param path Base path that is substituted. It's passed as const &.
     * @return Method returns the substituted path or the same path as provided, if no substitution took place.
     */
    static std::filesystem::path mapAddress( const std::filesystem::path& path );
};


