#include <sys/socket.h>
#include "CServer.h"
#include "CConfig.h"


void start(){
    CConfig conf;
    int srvrSocket = socket(AF_INET, SOCK_STREAM,0); // domain, type, protocol
    int bind(srvrSocket, conf.data["network"]["port"], )
};

void shutdown();