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

#define SERVERPORT 4444

int check(int exp, const char* msg) {
	if (exp == -1) {
		perror(msg);
		exit(1);
	}
	return exp;
}

void* handle_connection(void* ptr_client_socket) {
	int client_socket = *((int*)ptr_client_socket);
	free(ptr_client_socket);

	// read client's input
	FILE* file_ptr = fopen("file.txt", "a");
	int number, n;
	unsigned long long int factorial = 1;
	check(write(client_socket, "Enter the number: ", strlen("Enter the number: ")), "Write Failed.\n");
	read(client_socket, &number, sizeof(int));

	for (int j = 2; j <= number; ++j)   
		factorial *= j;
	
	// char address[INET_ADDRSTRLEN];
	// inet_ntop(AF_INET, &server_addr.sin_addr, address, sizeof(address));    
	fprintf(file_ptr, "Factorial of %d: %llu\n", number, factorial);
	write(client_socket, &factorial, sizeof(unsigned long long int ));
	fclose(file_ptr);
	return NULL;
}

int main(int argc, char** argv){
    
	remove("file.txt");
	
	int server_socket, client_socket, addr_size;
	struct sockaddr_in server_addr, client_addr;

	check(server_socket = socket(AF_INET, SOCK_STREAM, 0), "Server Socket Creation Failed.\n");
	
	// initialize address struct
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVERPORT);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	
	check(bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)), "Binding Failed.\n");
	
	check(listen(server_socket, 10), "Listen Failed");
	
	while (true) {
		addr_size = sizeof(struct sockaddr_in);
		check(client_socket = accept(server_socket, (struct sockaddr*)&client_addr, (socklen_t*) &addr_size), "Accept Failed");
		
		// printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

		pthread_t t;
		int* ptr_client_socket = malloc(sizeof(int));
		*ptr_client_socket = client_socket;
		pthread_create(&t, NULL, handle_connection, ptr_client_socket);
		pthread_join(t, NULL);
	}
	close(server_socket);
	return 0;
}