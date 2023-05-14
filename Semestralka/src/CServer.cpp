#include <csignal>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <map>
#include <sys/socket.h>
#include <netinet/in.h>

#include "CServer.h"
#include "CConfig.h"
#include "CLogger.h"
#include "CUtils.h"

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
        // accept a connection
        int cliSocket = accept( srvrSocket, (struct sockaddr *) &remote_address, &socklen);
        if( cliSocket < 0 ){
            close(srvrSocket);
            throw std::runtime_error("Connection couln't be made");
        }
        logger.log("Connection made");

        // recieve data from connection
        char buffer[BUFFER_SIZE];
        while(true){
            unsigned int bytesRead = recv(cliSocket, buffer, BUFFER_SIZE - 1, 0);
            buffer[bytesRead] = '\0';
            std::cout << buffer << std::endl;

            if (buffer == std::string("konec"))
                exit(EXIT_SUCCESS);
        }


    }
}

std::vector<std::string> CServer::parse(){
    std::string input;
    std::vector<std::string> parsed;
    size_t first;
    getline(std::cin, input, '\n');
    while(true){
        first = input.find(' ');
        if(first == std::string::npos){
            parsed.push_back(input);
            break;
        }
        parsed.push_back(input.substr(0,first));
        input.erase(0,first + 1);
    }

    return parsed;
}

void CServer::console(){
    std::map <std::string, CUtils* > utils;
    std::vector<std::string> parsed;

    utils = {{"display", new(CDisplay)},
             {"content", new(CContent)},
             {"execute", new(CExecute)},
             {"config", new(CChangeConfig)}};
    while(true){
        parsed = CServer::parse();


        if( utils.find(parsed[0]) == utils.end()){
            std::cout << "Unknown command" << std::endl << std::endl;
            continue;
        }

        if( parsed.size() < 2){
            std::cout << "No arguments" << std::endl << std::endl;
            continue;
        }

        for(unsigned int i = 1; i < parsed.size(); i ++)
            utils[parsed[0]]->launch(parsed[i]);

    }
}


void CServer::shutdown( int srvrSocket, int cliSocket = 0 ){
    close(srvrSocket);
    if(cliSocket > 0)
        close(cliSocket);
    exit(EXIT_SUCCESS);
}