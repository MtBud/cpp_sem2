#include <iostream>
using namespace std;

#include <cstdlib>
#include <cstdio>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h> // struct sockaddr_in
#include <strings.h> // bzero()
#include <netdb.h>
#include <string.h>


int main() {
    // Vytvoreni koncoveho bodu spojeni
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if(s < 0){
        perror("Nemohu vytvorit socket: ");
        return -1;
    }

    socklen_t sockAddrSize;
    sockAddrSize = sizeof(struct sockaddr_in);

    struct sockaddr_in serverAddr;
    bzero((char *) &serverAddr, sockAddrSize);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);

    struct hostent *host;
    host = gethostbyname("localhost"); // <netdb.h>
    memcpy(&serverAddr.sin_addr, host->h_addr,
           host->h_length); // <string.h>

    // Pripojeni ke vzdalenemu serveru
    if(connect(s, (struct sockaddr *) &serverAddr, sockAddrSize) < 0){
        perror("Nemohu spojit!");
        close(s);
        return -1;
    }

#define BUFFER_SIZE 100240
    char buffer[BUFFER_SIZE];
    while(true){
        cin.getline (buffer, BUFFER_SIZE - 1);
        // Priznak MSG_NOSIGNAL znamena, ze v pripade predcasneho ukonceni
        // na druhe strane nedojde k preruseni aktulaniho procesu signalem
        // SIGPIPE, ale funkce send() skonci s nenulovou navratovou hodnotou
        // a my muzeme korektne ukoncit nas program.
        if(send(s, buffer, strlen(buffer), MSG_NOSIGNAL) < 0){
            perror("Nemohu odeslat data:");
            close(s);
            return -3;
        }
    }


    close(s);
    return 0;
}
