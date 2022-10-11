#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 4444

int check(int ret, const char* message) {
	if (ret < 0) {
		perror(message);
		exit(1);
	}
	return ret;
}

int main(){
    remove("file.txt");
	clock_t tic = clock();
	
	int server_socket, ret, client_socket;
	struct sockaddr_in server_addr, client_addr;
	socklen_t addr_size;
	char buffer[1024];
	pid_t childpid;

	check(server_socket = socket(AF_INET, SOCK_STREAM, 0), "ServerSocket Creation Failed.\n");

	memset(&server_addr, '\0', sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	check(bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)), "Binding Failed.\n");

	check(listen(server_socket, 10), "Listen Failed.\n");

    int t = 10;
	while(t > 0){
        t = t - 1;
		check(client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_size), "Accept Failed.\n");
		
		printf("Connection accepted from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

		if((childpid = fork()) == 0){
			close(server_socket);
            FILE* file_ptr = fopen("file.txt", "a");
            int number;
            unsigned long long int factorial = 1;
            check(write(client_socket, "Enter the number: ", strlen("Enter the number: ")), "Write Failed.\n");
    
            read(client_socket, &number, sizeof(int));

            for (int j = 2; j <= number; ++j)   factorial *= j;
            
            char address[INET_ADDRSTRLEN];
            inet_ntop( AF_INET, &server_addr.sin_addr, address, sizeof(address));    
            fprintf(file_ptr, "Address: %s, Port: %d, Factorial of %d: %llu\n", address, htons(server_addr.sin_port), number, factorial);
            write(client_socket, &factorial, sizeof(unsigned long long int ));
		}
	}

	close(client_socket);

	clock_t toc = clock();
    printf("Time Elapsed: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC);
	return 0;
}