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

int check(int exp, const char* msg) {
	if (exp == -1) {
		perror(msg);
		exit(1);
	}
	return exp;
}

void* routine(void* ptr_i) {
	int i = *((int*)ptr_i);
	free(ptr_i);

    int socket_fd, ret, n;
	struct sockaddr_in serverAddr;
	char buffer[1024];

	check(socket_fd = socket(AF_INET, SOCK_STREAM, 0), "Client Socket Creation Failed");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = INADDR_ANY;

	check(connect(socket_fd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)), "Connect Failed");
	
    check(read(socket_fd, buffer, 255), "Read Failed");
    
    unsigned long long int  factorial;
    write(socket_fd, &i, sizeof(int));
    read(socket_fd, &factorial, sizeof(unsigned long long int ));
    printf("Factorial of %d = %llu\n", i, factorial);
}

int main(int arc, char** argv){
    for (int i = 1; i <= 10; ++i) {
        pthread_t t;
		int* ptr_i = malloc(sizeof(int));
		*ptr_i = i;
        pthread_create(&t, NULL, routine, ptr_i);
    }
	return 0;
}