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

int check(int ret, const char* message) {
	if (ret < 0) {
		perror(message);
		exit(1);
	}
	return ret;
}

void* routine(void* ptr_i) {
	
	int i = *((int*)ptr_i);
	free(ptr_i);

    int client_socket;
	struct sockaddr_in server_addr;
	char buffer[1024];

	check(client_socket = socket(AF_INET, SOCK_STREAM, 0), "Client Socket Creation Failed");

	memset(&server_addr, '\0', sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	check(connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)), "Connect Failed");
	
    check(read(client_socket, buffer, 255), "Read Failed");
    
    unsigned long long int  factorial;
    write(client_socket, &i, sizeof(int));
    read(client_socket, &factorial, sizeof(unsigned long long int));
    printf("Factorial of %d = %llu\n", i, factorial);
}

int main(int arc, char** argv){
    for (int i = 1; i <= 10; ++i) {
        pthread_t t;
		int* ptr_i = malloc(sizeof(int));
		*ptr_i = i;
        pthread_create(&t, NULL, routine, ptr_i);
		pthread_join(t, NULL);
    }
	return 0;
}