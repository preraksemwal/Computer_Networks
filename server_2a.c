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
    bzero((char*) &server_addr, sizeof(server_addr));  
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

    while (new_socket_fd != NULL) {
        int number, factorial = 1;
        n = write(new_socket_fd, "Enter the number: ", strlen("Enter the number: "));
        if (n < 0) {
            perror("Write Failed.\n");
            exit(1);
        }
        read(new_socket_fd, &number, sizeof(int));
        if (number == 11)
            break;
        while (number > 1) {
            factorial *= number--;
        }
        write(new_socket_fd, &factorial, sizeof(int));

        listen(socket_fd, 5);
        client_len = sizeof(client_addr);

        new_socket_fd = accept(socket_fd, (struct sockaddr*) &client_addr, &client_len);

        if (new_socket_fd < 0) {
            perror("Accept Failed.\n");
            exit(1);
        }
    }

    close(new_socket_fd);
    close(socket_fd);
    return 0;
}