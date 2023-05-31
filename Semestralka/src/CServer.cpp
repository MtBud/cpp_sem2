#include <csignal>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <map>
#include <set>
#include <sys/socket.h>
#include <netinet/in.h>

#include "CHTTPMethods.h"
#include "CServer.h"
#include "CConfig.h"
#include "CLogger.h"
#include "CUtils.h"

#define BUFFER_SIZE 8196



int CServer::start(){
    CConfig conf;

    // make a socket
    int srvrSocket = socket(AF_INET, SOCK_STREAM,0);
    if( srvrSocket < 0 )
        throw std::runtime_error("Socket initialization failed");

    // bind socket to port
    struct sockaddr_in sockAddress;
    sockAddress.sin_family = AF_INET;
    sockAddress.sin_port = htons(conf.data["network"]["port"]);
    sockAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    if( bind(srvrSocket, (struct sockaddr *) &sockAddress, sizeof(sockAddress)) < 0 ){
        close(srvrSocket);
        throw std::runtime_error("Port binding failed");
    }

    // listen on the socket
    if (listen(srvrSocket, 3) < 0)
    {
        close(srvrSocket);
        throw std::runtime_error("Cannot initiate listening");
    }

    CLogger::log("Server initialized");
    return srvrSocket;
}

std::vector<std::string> CServer::parse( std::string data, const std::string& delimiter ){
    std::vector<std::string> parsed;
    size_t delimSize = delimiter.size();
    while(true){
        size_t pos = data.find(delimiter);
        if(pos == std::string::npos){
            if( ! data.empty() )
                parsed.push_back(data);
            break;
        }
        parsed.push_back(data.substr(0,pos));
        data.erase(0,pos + delimSize);
    }

    return parsed;
}

void CServer::serve( int srvrSocket ){
    struct sockaddr_in remote_address;
    socklen_t socklen;
    std::map <std::string, CHTTPMethods* > methods;
    methods = {{"GET", new(CGet)},
               {"POST", new(CPost)}};

    while(true){
        // accept a connection
        int cliSocket = accept( srvrSocket, (struct sockaddr *) &remote_address, &socklen);
        if( cliSocket < 0 ){
            close(srvrSocket);
            throw std::runtime_error("Connection couln't be made");
        }
        CLogger::log("Connection made");

        // recieve data from connection
        char buffer[BUFFER_SIZE];
        while(true){
            std::stringstream message;
            std::string bytes;
            ssize_t bytesRead;
            while((bytesRead = recv(cliSocket, buffer, BUFFER_SIZE, 0)) > 0){
                bytes += std::string(buffer, bytesRead);
                if(bytesRead != BUFFER_SIZE)
                    break;
            }
            if( bytesRead == 0){
                CLogger::log("Connection ended abruptly");
                break;
            }

            CLogger::log("RECIEVED");
            CLogger::log(bytes);


            size_t requestEnd = bytes.find("\r\n\r\n");
            std::string requestBody = bytes.substr(0, requestEnd);
            std::string dataBody;
            if( requestEnd + 4 < bytes.size())
                dataBody = bytes.substr(requestEnd + 4);

            std::vector< std::string > request = parse( requestBody, "\r\n" );
            std::vector< std::string > requestLine = parse( request[0], " ");
            std::map< std::string, std::string > headers;
            // make map of headers
            for( unsigned int i = 1; i < request.size(); i++){
                std::vector< std::string > header;
                header = parse(request[i], ": ");
                if( header.size() == 1){
                    reply( cliSocket, CHTTPMethods::badRequest( "400 Bad Request", message));
                    continue;
                }
                headers.insert(std::pair(header[0], header[1] ) );
            }

            // check for bad requests
            if( CServer::requestSyntax( requestLine, headers, methods, cliSocket ) )
                continue;

            methods[requestLine[0]]->incoming( headers, requestLine[1], message, dataBody);
            size_t length = message.str().length();
            CLogger::log("SENDING");
            CLogger::log(message.str());
            send( cliSocket, message.str().c_str(), length, 0);
            if( headers["Connection"] == "close" ){
                CLogger::log("Connection closed by client");
                break;
            }
        }

    }
}

void CServer::shutdown( int srvrSocket, int cliSocket = 0 ){
    close(srvrSocket);
    if(cliSocket > 0)
        close(cliSocket);
    exit(EXIT_SUCCESS);
}

bool CServer::requestSyntax( const std::vector< std::string >& requestLine,
                             const std::map< std::string, std::string >& headers,
                             const std::map< std::string, CHTTPMethods* >& methods,
                             int cliSocket ){
    std::stringstream message;
    if( requestLine.size() != 3){
        reply( cliSocket, CHTTPMethods::badRequest( "400 Bad Request", message));
        std::cout << "Request line has a wrong number of parameters" << std::endl;
        return true;
    }

    if( methods.find(requestLine[0]) == methods.end()){
        reply( cliSocket, CHTTPMethods::badRequest( "400 Bad Request", message));
        std::cout << "Bad or unsupported method" << std::endl;
        return true;
    }

    if( std::set< std::string > {"HTTP/1.0", "HTTP/2", "HTTP/3", "HTTP/1.1"}.count( requestLine[2] ) == 0 ){
        reply( cliSocket, CHTTPMethods::badRequest( "400 Bad Request", message));
        std::cout << "Unrecognized HTTP version" << std::endl;
        return true;
    }

    return false;
}

void CServer::reply( int cliSocket, std::stringstream& message ){
    size_t length = message.str().length();
    send( cliSocket, message.str().c_str(), length, 0);
}