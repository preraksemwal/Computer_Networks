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
    // char buffer[255];
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
    
    // opens the file
    FILE* file_ptr = fopen("file.txt", "w");


    int number = 0;
    while (1) {
        unsigned long long int factorial = 1;
        n = write(new_socket_fd, "Enter the number: ", strlen("Enter the number: "));
        if (n < 0) {
            perror("Write Failed.\n");
            exit(1);
        }
        read(new_socket_fd, &number, sizeof(int));
        if (number > 20)
            break;
        for (int j = 2; j <= number; ++j)
            factorial *= j;

        
        char address[INET_ADDRSTRLEN];
        inet_ntop( AF_INET, &server_addr.sin_addr, address, sizeof(address));
        
        fprintf(file_ptr, "Address: %s, Port: %d, Factorial: %llu\n", address, htons(server_addr.sin_port), factorial);
        write(new_socket_fd, &factorial, sizeof(unsigned long long int ));
    }

    fclose(file_ptr);
    close(new_socket_fd);
    close(socket_fd);
    return 0;
}
// clear; gcc server.c -o server; ./server 9898