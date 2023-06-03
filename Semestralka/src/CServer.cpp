#include <csignal>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <map>
#include <set>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "CHTTPMethods.h"
#include "CPost.h"
#include "CGet.h"
#include "CServer.h"
#include "CConfig.h"
#include "CLogger.h"
#include "CUtils.h"

#define BUFFER_SIZE 8196


CServer::CServer():cliSocket(0), srvrSocket(0), get(nullptr), post(nullptr){}

int CServer::start(){
    CConfig conf;

    // make a socket
    srvrSocket = socket(AF_INET, SOCK_STREAM,0);
    if( srvrSocket < 0 )
        throw std::runtime_error("Socket initialization failed");

    // bind socket to port
    struct sockaddr_in sockAddress;
    sockAddress.sin_family = AF_INET;
    sockAddress.sin_port = htons(conf.data["network"]["port"]);
    if( 1 != inet_pton(AF_INET, std::string( conf.data["network"]["address"] ).c_str(), &(sockAddress.sin_addr)) ){
        close(srvrSocket);
        throw std::runtime_error("Invalid interface address");
    }
    //sockAddress.sin_addr.s_addr
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



void CServer::serve(){
    struct sockaddr_in remote_address;
    socklen_t socklen;

    std::map <std::string, CHTTPMethods* > methods;
    get = new(CGet);
    post = new(CPost);
    methods = {{ "GET", get },
               { "POST", post }};

    while(true){
        // accept a connection
        cliSocket = accept( srvrSocket, (struct sockaddr *) &remote_address, &socklen);
        if( cliSocket < 0 ){
            close(srvrSocket);
            throw std::runtime_error("Connection couln't be made");
        }
        CLogger::log("Connection made\n");

        // recieve data from connection
        char buffer[BUFFER_SIZE];
        while(true){
            std::stringstream message;
            std::string bytes;
            ssize_t bytesRead;

            // read data from socket
            while((bytesRead = recv(cliSocket, buffer, BUFFER_SIZE, 0)) > 0){
                bytes += std::string(buffer, bytesRead);
                if(bytesRead != BUFFER_SIZE)
                    break;
            }
            if( bytesRead == 0){
                CLogger::log("Connection ended abruptly\n");
                break;
            }

            // check for header field terminator
            size_t requestEnd = bytes.find("\r\n\r\n");
            if( requestEnd == std::string::npos ){
                reply( CHTTPMethods::badRequest( "400 Bad Request", message));
                CLogger::log("No header filed terminator");
                continue;
            }

            CLogger::log("RECIEVED");
            CLogger::log(std::string( bytes ).substr(0, bytes.find("\r\n\r\n") + 4));

            std::string requestBody = bytes.substr(0, requestEnd);
            std::string dataBody;
            if( requestEnd + 4 < bytes.size())
                dataBody = bytes.substr(requestEnd + 4);

            std::vector< std::string > request = parse( requestBody, "\r\n" );
            std::vector< std::string > requestLine = parse( request[0], " ");

            // make a map of headers
            bool flag = false;
            std::map< std::string, std::string > headers = parseHeaders( request, flag );
            if( flag ){
                reply( CHTTPMethods::badRequest( "400 Bad Request", message));
                continue;
            }

            // check for bad get_requests
            if( CServer::requestSyntax( requestLine, methods ) )
                continue;

            // pass the message to the corresponding method
            methods[requestLine[0]]->incoming( headers, requestLine[1], message, dataBody, cliSocket );
            reply( message );

            // close the connection if requested
            if( headers["Connection"] == "close" ){
                CLogger::log("Connection closed by client\n");
                break;
            }
        }

    }
}

//----------------------------------------------------------------------------------------------------------------------

void CServer::shutdown() const{
    if( cliSocket > 0 )
        close( cliSocket );
    close( srvrSocket );
    delete get;
    delete post;
}

//----------------------------------------------------------------------------------------------------------------------

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

//----------------------------------------------------------------------------------------------------------------------

std::map< std::string, std::string > CServer::parseHeaders( const std::vector< std::string >& request, bool& flag ){
    std::map< std::string, std::string > headers;
    for( unsigned int i = 1; i < request.size(); i++){
        std::vector< std::string > header;
        header = parse(request[i], ": ");
        if( header.size() == 1){
            flag = true ;
        }
        headers.insert(std::pair(header[0], header[1] ) );
    }
    return headers;
}


//----------------------------------------------------------------------------------------------------------------------

bool CServer::requestSyntax( const std::vector< std::string >& requestLine,
                             const std::map< std::string, CHTTPMethods* >& methods ) const{
    std::stringstream message;
    if( requestLine.size() != 3){
        reply( CHTTPMethods::badRequest( "400 Bad Request", message));
        std::cout << "Request line has a wrong number of parameters" << std::endl;
        return true;
    }

    if( methods.find(requestLine[0]) == methods.end()){
        reply( CHTTPMethods::badRequest( "400 Bad Request", message));
        std::cout << "Bad or unsupported method" << std::endl;
        return true;
    }

    if( std::set< std::string > {"HTTP/2", "HTTP/3", "HTTP/1.1"}.count( requestLine[2] ) == 0 ){
        reply( CHTTPMethods::badRequest( "505 HTTP Version Not Supported", message));
        std::cout << "Unrecognized HTTP version" << std::endl;
        return true;
    }

    return false;
}

//----------------------------------------------------------------------------------------------------------------------

void CServer::reply( std::stringstream& message ) const{
    size_t length = message.str().length();
    CLogger::log("SENDING");
    CLogger::log(message.str().substr(0, message.str().find("\r\n\r\n") + 4));
    send( cliSocket, message.str().c_str(), length, 0);
}