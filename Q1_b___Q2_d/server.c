#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <pthread.h>
#include <time.h>

#define SERVER_PORT 4444
struct sockaddr_in server_addr;

int check(int ret, const char* message) {
	if (ret < 0) {
		perror(message);
		exit(1);
	}
	return ret;
}

void* handle_connection(int client_socket) {
	// read client's input
	FILE* file_ptr = fopen("file.txt", "a");
	int number, n;
	unsigned long long int factorial = 1;
	check(write(client_socket, "Enter the number: ", strlen("Enter the number: ")), "Write Failed.\n");
	read(client_socket, &number, sizeof(int));

	for (int j = 2; j <= number; ++j)   
		factorial *= j;
	
	char address[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &server_addr.sin_addr, address, sizeof(address));  
	fprintf(file_ptr, "Address: %s, Port: %d, Factorial of %d: %llu\n", address, htons(server_addr.sin_port), number, factorial);  
	write(client_socket, &factorial, sizeof(unsigned long long int ));
	fclose(file_ptr);
	return NULL;
}

int accept_connection(int server_socket) {
    int client_socket;
    int addr_size = sizeof(struct sockaddr_in);
    struct sockaddr_in client_addr;
	check(client_socket = accept(server_socket, (struct sockaddr*)&client_addr, (socklen_t*) &addr_size), "Accept Failed");
    return client_socket;
}

int setup_server(short port, int backlog) {
    int server_socket, client_socket, addr_size;
	struct sockaddr_in client_addr;

	check(server_socket = socket(AF_INET, SOCK_STREAM, 0), "Server Socket Creation Failed.\n");
	
	// initialize address struct
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	
	check(bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)), "Binding Failed.\n");
	
	check(listen(server_socket, 10), "Listen Failed");
    return server_socket;
}

int main(int argc, char** argv){
	remove("file.txt");
	clock_t tic = clock();
    int server_socket = setup_server(SERVER_PORT, 10);

    fd_set current_sockets, ready_sockets;

    FD_ZERO(&current_sockets);
    FD_SET(server_socket, &current_sockets);

	while (1) {
        ready_sockets = current_sockets;
        check(select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL), "Select Failed");

        for (int i = 0; i < FD_SETSIZE; ++i) {
            if (FD_ISSET(i, &ready_sockets)) {
                if (i == server_socket) {
                    int client_socket = accept_connection(server_socket);
                    FD_SET(client_socket, &current_sockets);
                }
                else {
                    handle_connection(i);
                    FD_CLR(i, &current_sockets);
                }
            }
        }
	}
	close(server_socket);

	clock_t toc = clock();
    printf("Time Elapsed: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC);
	return EXIT_SUCCESS;
}