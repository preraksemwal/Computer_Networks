#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int main (int argc, char* argv[]) {
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent* server;
    char buffer[255];

    if (argc < 3) {
        fprintf(stderr, "Invalid Command Error.\n");
        exit(1);
    }

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd < 0) {
        error("Socket Failed.\n");
    }
    
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        error("Error no such host");
        exit(1);
    }
    bzero((char*) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char*) server->h_addr, (char*) &serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    
    if(connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
        error("Connection Failed.\n");
        exit(1);
    }

    for (int i = 1; i <= 10; ++i) {
        int number;
        printf("Enter request number %d:", i);
        scanf("%d", number);
        
        bzero(buffer, 255);
        fgets(buffer, 255, stdin);
        n = write(sockfd, buffer, strlen(buffer));
        if (n < 0) {
            error("Write Failed.\n");
        }
        bzero(buffer, 255);
        n = read(sockfd, buffer, 255);
        if (n < 0) {
            error("Read Failed.\n");
        }
        printf("Server: %s", buffer);

        int i = strncmp("Bye", buffer, 3);
    }

    return 0;
}