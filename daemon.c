#include<pthread.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>




struct LIST {
	int data[1024];
	int next_read, next_write;
	int count;
}accept_list;

void core(void *none);
int get_word(char *src, char *buffer, int index);

/*
	entry point of the program. 
	TODO: 
		make a socket server
		start threads to accept requests
*/
int main(int argc, const char **argv) {
	int sock_hdl = 0;
	
	sock_hdl = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_hdl == 0) {
		printf("EMERGENCY: socket failed (%d: %m)\n", errno);
		return 1;
	}
	
	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(80);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	
	
	if (bind(sock_hdl, (struct sockaddr *) &server_addr, sizeof(server_addr)) != 0) {
		printf("EMERGENCY: bind failed (%d: %m)\n", errno);
		return 1;
	}
	
	
	if (listen(sock_hdl, 10) != 0) {
		printf("EMERGENCY: listen failed (%d: %m)\n", errno);
		return 1;
	}
	
	/*
		okay, we have been make a socket server. now we create 4 threads to accept requests. 
	*/
	pthread_t thread;
	int error, i;
	
	for (i = 0; i < 8; i++) {
		error = pthread_create(&thread, NULL, (void *) core, NULL);
		if (error != 0) {
			printf("EMERGENCY: the %drd thread create failed (%d)\n", i, error);
			return 1;
		}
	}
	
	
	
	int client_hdl;
	struct sockaddr_in client_addr;
	int client_addr_length = sizeof(client_addr);
	int header_size;
	char *buffer = (char *) malloc(1024);
	
	for (;;) {
		client_hdl = accept(sock_hdl, (struct sockaddr *) &client_addr, &client_addr_length);
		printf("added 1 request to list\n");
		accept_list.data[accept_list.next_write] = client_hdl;
		accept_list.next_write++;
		if (accept_list.next_write == 1024)
			accept_list.next_write = 0;
		accept_list.count ++;
	}
}


/*
	get a request in list and accept it. 
*/
void core(void *none) {
	char buffer[1024];
	char request_method[10];
	int client_hdl;
	for (;;) {
		if (accept_list.count == 1){
			client_hdl = accept_list.data[accept_list.next_read];
			if (client_hdl == 0) 
				continue;
			accept_list.count--;
			printf("client handle: %d\n", client_hdl);
			if (accept_list.next_read == 1024) 
				accept_list.next_read = 0;
			accept_list.next_read++;
			
			recv(client_hdl, buffer, sizeof(buffer), 0);
			printf("%s", buffer);
			
			
			int request_method_len = get_word(buffer, request_method, 0);
			printf("request method: %s\n", request_method);
			
			if (strcmp(request_method, "GET") == 0) {
				// get request path
				char request_path[1024];
				get_word(buffer, request_path, request_method_len + 1);
				printf("request path: %s\n", request_path);
				continue;
			}
			else if (strcmp(request_method, "POST") == 0) {
				continue;
			}
		}
	}
}



/*
	get a word from 'src' (end with ' ')
*/
int get_word(char *src, char *buffer, int index) {
	int i;
	putchar(src[i]);
	for (i = index; src[i] != ' '; i++) {
		buffer[i] = src[i];
	}
	buffer[i] = 0;
	return i;
}
