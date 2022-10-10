#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 4444

int main(){
    remove("file.txt");

	int socket_fd, ret, new_socket_fd;
	struct sockaddr_in server_addr, newAddr;
	socklen_t addr_size;
	char buffer[1024];
	pid_t childpid;

	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_fd < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Server Socket is created.\n");

	memset(&server_addr, '\0', sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = bind(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if(ret < 0){
		printf("[-]Error in binding.\n");
		exit(1);
	}
	printf("[+]Bind to port %d\n", 4444);

	if(listen(socket_fd, 10) == 0){
		printf("[+]Listening....\n");
	}else{
		printf("[-]Error in binding.\n");
	}

    int t = 10;
	while(t > 0){
        t = t - 1;
		new_socket_fd = accept(socket_fd, (struct sockaddr*)&newAddr, &addr_size);
		if(new_socket_fd < 0){
			exit(1);
		}
		printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

		if((childpid = fork()) == 0){
			close(socket_fd);
            FILE* file_ptr = fopen("file.txt", "a");
            int number, n;
            unsigned long long int factorial = 1;
            n = write(new_socket_fd, "Enter the number: ", strlen("Enter the number: "));
            if (n < 0) {
                perror("Write Failed.\n");
                exit(1);
            }
            read(new_socket_fd, &number, sizeof(int));

            for (int j = 2; j <= number; ++j)   factorial *= j;
            
            char address[INET_ADDRSTRLEN];
            inet_ntop( AF_INET, &server_addr.sin_addr, address, sizeof(address));    
            fprintf(file_ptr, "Address: %s, Port: %d, Factorial of %d: %llu\n", address, htons(server_addr.sin_port), number, factorial);
            write(new_socket_fd, &factorial, sizeof(unsigned long long int ));
		}
	}

	close(new_socket_fd);
	return 0;
}