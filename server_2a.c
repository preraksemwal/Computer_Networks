#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main (int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Invalid Command Error.\n");
        exit(1);
    }
    int socket_fd, new_socket_fd, port_no, n;
    char buffer[255];
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);  
    if (socket_fd < 0) {
        perror("Socket Failed.\n");
        exit(1);
    }
    bzero((char*) &server_addr, sizeof(server_addr));  // clears any data in what its addressing to
    port_no = atoi(argv[1]);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port_no);

    if(bind(socket_fd, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0) {
        perror("Bind Failed.\n");
        exit(1);
    }

    listen(socket_fd, 5);
    client_len = sizeof(client_addr);

    new_socket_fd = accept(socket_fd, (struct sockaddr*) &client_addr, &client_len);

    if (new_socket_fd < 0) {
        perror("Accept Failed.\n");
        exit(1);
    }

    while (1) {
        bzero(buffer, 255);
        n = read(new_socket_fd, buffer, 255);  // will have correspondent client-write
        if (n < 0) {
            perror("Read Failed.\n");
            exit(1);
        }
        printf("Client: %s\n", buffer);
        bzero(buffer, 255);
        fgets(buffer, 255, stdin);

        n = write(new_socket_fd, buffer, strlen(buffer));  // will have correspondent client-read
        if (n < 0) {
            perror("Write Failed.\n");
            exit(1);
        }

        int i = strncmp("Bye", buffer, 3);
        if (i == 0)
            break;
    }

    close(new_socket_fd);
    close(socket_fd);
    return 0;
}