#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int main (int argc, char* argv[]) {
    int socket_fd, port_no, n;
    struct sockaddr_in server_addr;
    struct hostent* server;
    char buffer[255];

    if (argc < 3) {
        fprintf(stderr, "Invalid Command Error.\n");
        exit(1);
    }

    port_no = atoi(argv[2]);
    socket_fd = socket(AF_INET, SOCK_STREAM, 0); 
    if (socket_fd < 0) {
        perror("Socket Failed.\n");
    }
    
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        perror("Error no such host");
        exit(1);
    }
    bzero((char*) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    bcopy((char*) server->h_addr, (char*) &server_addr.sin_addr.s_addr, server->h_length);
    server_addr.sin_port = htons(port_no);
    
    if(connect(socket_fd, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0) {
        perror("Connection Failed.\n");
        exit(1);
    }
    for (int i = 1; i <= 21; ++i) {
        n = read(socket_fd, buffer, 255);
        if (n < 0) {
            perror("Read Failed");
            exit(1);
        }
        unsigned long long int  factorial;
        write(socket_fd, &i, sizeof(int));
        if (i > 20)
            break;
        read(socket_fd, &factorial, sizeof(unsigned long long int ));
        printf("Factorial of %d = %llu\n", i, factorial);
    }
    close(socket_fd);
    return 0;
}
// clear; gcc client.c -o client; ./client 127.0.0.1 9898