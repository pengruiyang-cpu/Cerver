#include "cerver.h"


/*
	entry point of the program. 
	TODO: 
		make a socket server
		start threads to accept requests
*/
int main(int argc, const char **argv) {
	int sock_hdl = 0;
        accept_list.free = LIST_COUNT;
	accept_list.count = 0;

	sock_hdl = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_hdl == 0) {
		printf("EMERGENCY: socket failed (%d: %m)\n", errno);
		return 1;
	}
	DEBUG_LOG("make socket done\n");
	
	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(80);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	
	
	if (bind(sock_hdl, (struct sockaddr *) &server_addr, sizeof(server_addr)) != 0) {
		printf("EMERGENCY: bind failed (%d: %m)\n", errno);
		return 1;
	}
	DEBUG_LOG("bind IP and PORT done\n");
	
	
	if (listen(sock_hdl, 10) != 0) {
		printf("EMERGENCY: listen failed (%d: %m)\n", errno);
		return 1;
	}
	DEBUG_LOG("listen done\n");
	
	// false
	core_is_sleeping = 0;

	/*
		okay, we have been make a socket server. now we create a core thread to controll all accept-threads. 
	*/
	pthread_t thread_core;
	int error;

	if (pthread_create(&thread_core, NULL, (void *) core, NULL) != 0) {
		printf("EMERGENCY: core-thread create failed (%d)\n", error);
		return 1;
	}
	DEBUG_LOG("core-thread create done\n");
	
	
	
	
	int client_hdl;
	struct sockaddr_in client_addr;
	int client_addr_length = sizeof(client_addr);
	int header_size;
	char *buffer = (char *) malloc(1024);

	for (;;) {
		client_hdl = accept(sock_hdl, (struct sockaddr *) &client_addr, &client_addr_length);
		DEBUG_LOG("added 1 request to list\n");
		accept_list.data[accept_list.next_write] = client_hdl;
		accept_list.next_write++;
		if (accept_list.next_write == LIST_COUNT)
			accept_list.next_write = 0;
		accept_list.count++;
                accept_list.free--;
		if (core_is_sleeping == 1) {
			if (pthread_create(&thread_core, NULL, (void *) core, NULL) != 0) {
				printf("EMERGENCY: core-thread wake-up failed (%d)\n", error);
				return 1;
			}
			DEBUG_LOG("core-thread wake-up\n");
		}
                if (accept_list.free == 0) {
                        printf("EMERGENCY: accept list.free == 0. abadon\n");
                }
                printf("accept list has %d free\n", accept_list.free);
	}
}
