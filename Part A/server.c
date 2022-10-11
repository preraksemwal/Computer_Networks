#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int check(int ret, const char* message) {
	if (ret < 0) {
		perror(message);
		exit(1);
	}
	return ret;
}

int main (int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Invalid Command Error.\n");
        exit(1);
    }
    int server_socket, client_socket, port_no, n;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;

    check(server_socket = socket(AF_INET, SOCK_STREAM, 0), "Server Socket Creation Failed.\n");
    
    bzero((char*) &server_addr, sizeof(server_addr));  
    port_no = atoi(argv[1]);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port_no);

    check(bind(server_socket, (struct sockaddr*) &server_addr, sizeof(server_addr)), "Binding Failed.\n");

    listen(server_socket, 5);
    client_len = sizeof(client_addr);

    check(client_socket = accept(server_socket, (struct sockaddr*) &client_addr, &client_len), "Accept Failed.\n");
    
    // opens the file
    FILE* file_ptr = fopen("file.txt", "w");

    int number = 0;
    while (1) {
        unsigned long long int factorial = 1;
        check(write(client_socket, "Enter the number: ", strlen("Enter the number: ")), "Write Failed.\n");
        
        read(client_socket, &number, sizeof(int));
        if (number > 20)
            break;
        for (int j = 2; j <= number; ++j)
            factorial *= j;

        
        char address[INET_ADDRSTRLEN];
        inet_ntop( AF_INET, &server_addr.sin_addr, address, sizeof(address));
        
        fprintf(file_ptr, "Address: %s, Port: %d, Factorial of %d: %llu\n", address, htons(server_addr.sin_port), number, factorial);
        write(client_socket, &factorial, sizeof(unsigned long long int ));
    }

    fclose(file_ptr);
    close(client_socket);
    close(server_socket);
    return 0;
}
// clear; gcc server.c -o server; ./server 9898