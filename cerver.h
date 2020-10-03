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
#include<stdarg.h>


#define DEBUG_LOG(info) printf("%s: %s", __FUNCTION__, info);


struct LIST {
	int data[1024];
	int next_read, next_write;
	int count;
}accept_list;

// 'void *none' is a placeholder

// core.c
void core(void *none);

// accept.c
void accept_request(void *none);

int core_is_sleeping;

