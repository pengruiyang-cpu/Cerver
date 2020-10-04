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
#include "settings.h"


#define DEBUG_LOG(info) printf("[%s]: %s", __FUNCTION__, info);
#define DEBUG_LOG_AR(info, no) printf("[%s %d]: %s", __FUNCTION__, no, info);


struct LIST {
	int data[LIST_COUNT];
	int next_read, next_write;
	unsigned int count;
        unsigned int free;
}accept_list;

// 'void *none' is a placeholder

// core.c
void core(void *none);

// accept.c
void accept_request(int *no);

int core_is_sleeping;

