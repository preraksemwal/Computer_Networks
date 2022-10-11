#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int check(int ret, const char* message) {
	if (ret < 0) {
		perror(message);
		exit(1);
	}
	return ret;
}

int main (int argc, char* argv[]) {
    int client_socket, port_no;
    struct sockaddr_in server_addr;
    struct hostent* server;
    char buffer[255];

    if (argc < 3) {
        fprintf(stderr, "Invalid Command Error.\n");
        exit(1);
    }

    port_no = atoi(argv[2]);
    check(client_socket = socket(AF_INET, SOCK_STREAM, 0), "Client Socket Creation Failed.\n"); 
    
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        perror("Error no such host");
        exit(1);
    }
    bzero((char*) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    bcopy((char*) server->h_addr, (char*) &server_addr.sin_addr.s_addr, server->h_length);
    server_addr.sin_port = htons(port_no);
    
    check(connect(client_socket, (struct sockaddr*) &server_addr, sizeof(server_addr)), "Connect Failed.\n");

    for (int i = 1; i <= 21; ++i) {
        check(read(client_socket, buffer, 255), "Read Failed.\n");
    
        unsigned long long int  factorial;
        write(client_socket, &i, sizeof(int));
        if (i > 20)
            break;
        read(client_socket, &factorial, sizeof(unsigned long long int ));
        printf("Factorial of %d = %llu\n", i, factorial);
    }
    close(client_socket);
    return 0;
}
// clear; gcc client.c -o client; ./client 127.0.0.1 9898