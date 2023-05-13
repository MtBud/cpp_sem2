#include <csignal>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>

#include "CServer.h"
#include "CConfig.h"
#include "CLogger.h"

#define BUFFER_SIZE 10240



int CServer::start(){
    CConfig conf;
    CLogger logger;

    // make a socket
    int srvrSocket = socket(AF_INET, SOCK_STREAM,0);
    if( srvrSocket < 0 )
        throw std::runtime_error("Socket initialization failed");

    // bind socket to port
    struct sockaddr_in sockAddress;
    bzero(&sockAddress, sizeof(sockAddress));
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

    logger.log("Server initialized");

    return srvrSocket;
}

void CServer::serve( int srvrSocket ){
    CLogger logger;
    struct sockaddr_in remote_address;
    socklen_t socklen;

    while(true){
        int cliSocket = accept( srvrSocket, (struct sockaddr *) &remote_address, &socklen);
        if( cliSocket < 0 ){
            close(srvrSocket);
            throw std::runtime_error("Connection couln't be made");
        }
        logger.log("Connection made");


        char buffer[BUFFER_SIZE];
        while(true){
            int bytesRead = recv(cliSocket, buffer, BUFFER_SIZE - 1, 0);
            buffer[bytesRead] = '\0';
            std::cout << buffer << std::endl;

            if (buffer == std::string("konec"))
                exit(EXIT_SUCCESS);
        }


    }
}

void CServer::shutdown(){

}