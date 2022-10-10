#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 4444

void* routine(int i) {
    printf("%d", i);
    int socket_fd, ret, n;
	struct sockaddr_in serverAddr;
	char buffer[1024];

	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_fd < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Client Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	// serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    bcopy((char*) server->h_addr, (char*) &server_addr.sin_addr.s_addr, server->h_length);

	ret = connect(socket_fd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Connected to Server.\n");

    n = read(socket_fd, buffer, 255);
    if (n < 0) {
        perror("Read Failed");
        exit(1);
    }
    unsigned long long int  factorial;
    write(socket_fd, &i, sizeof(int));
    if (i > 20)
        return;
    read(socket_fd, &factorial, sizeof(unsigned long long int ));
    printf("Factorial of %d = %llu\n", i, factorial);
}

int main(){
    for (int i = 1; i <= 10; ++i) {
        pthread_t t;
        pthread_create(&t, NULL, &routine, i);
        pthread_join(t, NULL);
    }
	return 0;
}