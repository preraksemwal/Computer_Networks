#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char* message) {
    perror(message);
    exit(1);
}

int main (int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Invalid Command Error.\n");
        exit(1);
    }
    int sockfd, newsockfd, portno, n;
    char buffer[255];

    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);  // for TCP
    if (sockfd < 0) {
        error("Socket Failed.\n");
    }
    bzero((char*) &serv_addr, sizeof(serv_addr));  // clears any data in what its addressing to
    portno = atoi(argv[1]);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);  // host-to-network-short

    if(bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
        error("Bind Failed.\n");
    }

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    newsockfd = accept(sockfd, (struct sockaddr*) &cli_addr, &clilen);

    if (newsockfd < 0) {
        error("Accept Failed.\n");
    }

    while (1) {
        bzero(buffer, 255);
        n = read(newsockfd, buffer, 255);  // will have correspondent client-write
        if (n < 0) {
            error("Read Failed.\n");
        }
        printf("Client: %s\n", buffer);
        bzero(buffer, 255);
        fgets(buffer, 255, stdin);

        n = write(newsockfd, buffer, strlen(buffer));  // will have correspondent client-read
        if (n < 0) {
            error("Write Failed.\n");
        }

        int i = strncmp("Bye", buffer, 3);
        if (i == 0)
            break;
    }

    close(newsockfd);
    close(sockfd);
    return 0;
}