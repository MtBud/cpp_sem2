#include "CServer.h"
int main(){
    CServer server;
    int srvrSocket = server.start();
    server.serve( srvrSocket );

    return 1;
}